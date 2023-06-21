/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
*
* This file is part of libcitygml library
* http://code.google.com/p/libcitygml
*
* libcitygml is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2.1 of the License, or
* (at your option) any later version.
*
* libcitygml is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*/

#include <fstream>
#include <iterator> // MT 15/02/2016 (vs2015)

#include "Polygon.h"

#include "citygmls/Geometry.h"
#include "citygmls/Appearance.h"
#include "citygmls/AppearanceManager.h"
#include "Tesselator.h"
#include "Material.h"
#include "Texture.h"
#include "GeoReferencedTexture.h"

#include "LinearRing.h"

#ifndef min
# define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

Polygon::Polygon( const std::string& id )
  : Object( id ), _appearance( 0 ), _texture( 0 ), _exteriorRing( 0 ), _negNormal( false ), _geometry( 0 )
{
  _materials[ FRONT ] = 0;
  _materials[ BACK ] = 0;
}

Polygon::~Polygon()
{
  delete _exteriorRing;
  std::vector< LinearRing* >::const_iterator it = _interiorRings.begin();
  for ( ; it != _interiorRings.end(); ++it ) delete *it;
}

//Polygon* Polygon::Clone()
//{
//  LinearRing* ExtRing = this->getExteriorRing();
//
//  Polygon * Poly = new Polygon(this->getId());
//  LinearRing * NewExtRing = new LinearRing(ExtRing->getId(), true);
//
//  for(glm::highp_dvec3 P : ExtRing->getVertices())
//  {
//    NewExtRing->addVertex(P);
//  }
//  Poly->addRing(NewExtRing);
//
//  for(LinearRing* IntRing : this->getInteriorRings())
//  {
//    LinearRing * NewIntRing = new LinearRing(IntRing->getId(), false);
//
//    for(glm::highp_dvec3 P : ExtRing->getVertices())
//    {
//      NewIntRing->addVertex(P);
//    }
//    Poly->addRing(NewIntRing);
//  }
//
//  return Poly;
//}

const std::vector<glm::highp_dvec3>& Polygon::getVertices() const
{
  return _vertices;
}

// Get the indices
const std::vector<unsigned int>& Polygon::getIndices() const
{
  return _indices;
}

// Get the normals
const std::vector<glm::vec3>& Polygon::getNormals() const
{
  return _normals;
}

// Get texture coordinates
TexCoords& Polygon::getTexCoords()
{
  return _texCoords;
}

const TexCoords& Polygon::getTexCoords() const
{
  return _texCoords;
}

const Appearance* Polygon::getAppearance() const
{
  return _appearance;
} // Deprecated! Use getMaterial and getTexture instead

const Material* Polygon::getMaterial() const
{
  if ( _materials[ FRONT ] ) { return _materials[ FRONT ]; }
  else { return _materials[ BACK ]; }
}

const Texture* Polygon::getTexture() const
{
  return _texture;
}

const Material* Polygon::getMaterialFront() const
{
  return _materials[ FRONT ];
}

const Material* Polygon::getMaterialBack() const
{
  return _materials[ BACK ];
}

const std::vector<LinearRing*>& Polygon::getInteriorRings() const
{
  return _interiorRings;
}

std::vector<LinearRing*>& Polygon::getInteriorRings()
{
  return _interiorRings;
}

const LinearRing* Polygon::getExteriorRing() const
{
  return _exteriorRing;
}

LinearRing* Polygon::getExteriorRing()
{
  return _exteriorRing;
}

const Envelope& Polygon::getEnvelope() const
{
  return _envelope;
}

glm::highp_dvec3 Polygon::computeNormal()
{
  if ( !_exteriorRing ) return glm::highp_dvec3();

  glm::highp_dvec3 normal = _exteriorRing->computeNormal();

  return _negNormal ? -normal : normal;
}

void Polygon::tesselate( AppearanceManager &appearanceManager, const glm::highp_dvec3& normal )
{
  _indices.clear();

  if ( !_exteriorRing || _exteriorRing->size() < 3 )
  {
    mergeRings( appearanceManager );
    return;
  }

  TexCoords texCoords;
  bool t = appearanceManager.getTexCoords( _exteriorRing->getId(), texCoords );
  _exteriorRing->finish( t ? &texCoords : &_texCoords );
  if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );

  for ( unsigned int i = 0; i < _interiorRings.size(); i++ ) {
    TexCoords texCoords;
    bool t = appearanceManager.getTexCoords( _interiorRings[i]->getId(), texCoords );
    _interiorRings[i]->finish( t ? &texCoords : &_texCoords );
    if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );
  }

  // Compute the total number of vertices
  size_t vsize = _exteriorRing->size();
  for ( size_t i = 0; i < _interiorRings.size(); i++ )
    vsize += _interiorRings[i]->size();

  Tesselator* tess = appearanceManager.getTesselator();
  tess->init( vsize, normal );

  tess->addContour( _exteriorRing->getVertices(), texCoords );

  for ( size_t i = 0; i < _interiorRings.size(); i++ )
    tess->addContour( _interiorRings[i]->getVertices(), texCoords );

  tess->compute();
  _vertices.reserve( tess->getVertices().size() );
  std::copy( tess->getVertices().begin(), tess->getVertices().end(), std::back_inserter( _vertices ) );

  size_t indicesSize = tess->getIndices().size();
  if ( indicesSize > 0 )
  {
    _indices.resize( indicesSize );
    memcpy( &_indices[0], &tess->getIndices()[0], indicesSize * sizeof(unsigned int) );
  }
  clearRings();
}

void Polygon::mergeRings( AppearanceManager &appearanceManager )
{
  _vertices.reserve( _vertices.size() + _exteriorRing->size() );
  TexCoords texCoords;
  bool t = appearanceManager.getTexCoords( _exteriorRing->getId(), texCoords );
  _exteriorRing->finish( t ? &texCoords : &_texCoords );
  if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );

  std::copy( _exteriorRing->getVertices().begin(), _exteriorRing->getVertices().end(), std::back_inserter( _vertices ) );

  for ( size_t i = 0; i < _interiorRings.size(); i++ )
  {
    TexCoords texCoords;
    bool t = appearanceManager.getTexCoords( _interiorRings[i]->getId(), texCoords );
    _interiorRings[i]->finish( t ? &texCoords : &_texCoords );
    if ( t ) std::copy( texCoords.begin(), texCoords.end(), std::back_inserter( _texCoords ) );

    _vertices.reserve( _vertices.size() + _interiorRings[i]->size() );

    std::copy( _interiorRings[i]->getVertices().begin(), _interiorRings[i]->getVertices().end(), std::back_inserter( _vertices ) );
  }
  clearRings();
  _indices.clear();

  if ( _vertices.size() < 3 ) return;

  // Create triangles' indices
  size_t indicesSize = 3 * ( _vertices.size() - 2 );
  if ( indicesSize < 3 )
    return;
  _indices.resize( indicesSize );
  for ( size_t i = 0, p = 0; p < indicesSize - 2; i++, p += 3 )
    for ( size_t j = 0; j < 3; j++ )
      _indices[ p + j ] = (int)(i + j);
}

void Polygon::clearRings()
{
  return;
  delete _exteriorRing;
  _exteriorRing = 0;
  for ( size_t i = 0; i < _interiorRings.size(); i++ ) delete _interiorRings[i];
  _interiorRings.clear();
}

// Merge polygon p into the current polygon
bool Polygon::merge( Polygon* p )
{
  if ( !p ) return false;

  if ( p->getAppearance() != getAppearance() ) return false;

  if ( p->getVertices().size() == 0 ) return true;

  // merge vertices
  size_t oldVSize = _vertices.size();
  size_t pVSize = p->_vertices.size();
  _vertices.resize( oldVSize + pVSize );
  for ( size_t i = 0; i < pVSize; i++ )
    _vertices[ oldVSize + i ] = p->_vertices[i];
  p->_vertices.clear();

  // merge indices
  {
    size_t oldSize = _indices.size();
    size_t pSize = p->_indices.size();
    _indices.resize( oldSize + pSize );
    for ( size_t i = 0; i < pSize; i++ )
      _indices[ oldSize + i ] = (int)(oldVSize + p->_indices[i]);
    p->_indices.clear();
  }

  // merge normals
  {
    size_t oldSize = _normals.size();
    size_t pSize = p->_normals.size();
    _normals.resize( oldSize + pSize );
    for ( size_t i = 0; i < pSize; i++ )
      _normals[ oldSize + i ] = p->_normals[i];
    p->_normals.clear();
  }

  // merge texcoords
  {
    size_t oldSize = min( _texCoords.size(), oldVSize );
    size_t pSize = min( p->_texCoords.size(), pVSize );
    _texCoords.resize( oldSize + pSize );
    for ( size_t i = 0; i < pSize; i++ )
      _texCoords[ oldSize + i ] = p->_texCoords[i];
    p->_texCoords.clear();
  }

  // merge ids
  _id += "+" + p->_id;

  return true;
}

void Polygon::finish( AppearanceManager& appearanceManager, bool doTesselate )
{
  glm::highp_dvec3 normal = computeNormal();
  if ( doTesselate ) tesselate( appearanceManager, normal );  else mergeRings( appearanceManager );

  // Create the normal per point field
  _normals.resize( _vertices.size() );
  for ( size_t i = 0; i < _vertices.size(); i++ )
    _normals[i] = glm::vec3( (float)normal.x, (float)normal.y, (float)normal.z );
}

void Polygon::finish( AppearanceManager& appearanceManager, Appearance* defAppearance, bool doTesselate )
{
  if ( !appearanceManager.getTexCoords( getId(), _texCoords ) )
    appearanceManager.getTexCoords( _geometry->getId(), _texCoords );

  finish( appearanceManager, doTesselate );

  _texCoords.resize( _vertices.size() );

  const std::string id = getId();
  _appearance = appearanceManager.getAppearance( id );
  if ( !_appearance ) _appearance = defAppearance;

  _materials[ FRONT ] = appearanceManager.getMaterialFront( id );
  _materials[ BACK ] = appearanceManager.getMaterialBack( id );
  if ( !_materials[ FRONT ]  && !_materials[ BACK ])
    _materials[ FRONT ] = dynamic_cast< Material * >( defAppearance );

  // handle georeferencedtextures here ?
  //GeoreferencedTexture* geoTexture = appearanceManager.getGeoreferencedTexture(m_matId);
  GeoreferencedTexture* geoTexture = dynamic_cast<GeoreferencedTexture*>(defAppearance);
  if(geoTexture)
  {
    //std::cout << "has GeoreferencedTexture : " << m_matId << std::endl;
    _texture = geoTexture;

    if(!geoTexture->m_initWParams)
    {
      // open world file file
      std::string basePath = appearanceManager.m_basePath;
      //std::string basePath = "/mnt/docs/data/dd_backup/Donnees_GrandLyon/MNT_CITYGML/";
      //std::string basePath = "/mnt/docs/data/dd_backup/Donnees_Sathonay/";
      std::string worldFileUrl(basePath);
      worldFileUrl.append(_texture->getUrl());
      char lastChar = worldFileUrl.back();
      worldFileUrl.pop_back();
      worldFileUrl.pop_back();
      worldFileUrl.push_back(lastChar);
      worldFileUrl.push_back('w');
      //worldFileUrl = worldFileUrl.substr(0, worldFileUrl.find_last_of('.')) + ".jgw";
      //std::cout << "worldFileUrl : " << worldFileUrl << std::endl;
      std::ifstream worldFile(worldFileUrl);

      worldFile >> geoTexture->m_wParams.xPixelSize;
      worldFile >> geoTexture->m_wParams.yRotation;
      worldFile >> geoTexture->m_wParams.xRotation;
      worldFile >> geoTexture->m_wParams.yPixelSize;
      worldFile >> geoTexture->m_wParams.xOrigin;
      worldFile >> geoTexture->m_wParams.yOrigin;

      //std::cout << geoTexture->m_wParams;

      worldFile.close();

      geoTexture->m_initWParams = true;
    }

    // compute tex coords
    _texCoords.clear();
    GeoreferencedTexture::WorldParams& wParams = geoTexture->m_wParams;
    const std::vector<glm::highp_dvec3>& vertices = _exteriorRing->getVertices();
    for(std::vector<glm::highp_dvec3>::const_iterator it = vertices.begin(); it < vertices.end(); ++it)
    {
      glm::highp_dvec3 point = *it;
      glm::highp_dvec2 tc;

      tc.x = ((wParams.yPixelSize*point.x)-(wParams.xRotation*point.y)+(wParams.xRotation*wParams.yOrigin)-(wParams.yPixelSize*wParams.xOrigin)) / ((wParams.xPixelSize*wParams.yPixelSize)-(wParams.yRotation*wParams.xRotation));
      tc.y = ((-wParams.yRotation*point.x)+(wParams.xPixelSize*point.y)+(wParams.yRotation*wParams.xOrigin)-(wParams.xPixelSize*wParams.yOrigin)) / ((wParams.xPixelSize*wParams.yPixelSize)-(wParams.yRotation*wParams.xRotation));

      tc.y = 1.0 - tc.y;

      // normalize later ? when converting to osg (because we can have image size at this time) ?
      //*
      //tc.x /= 4096.0f;
      //tc.y /= 4096.0f;
      /*/
      tc.x /= 8192.0f;
      tc.y /= 8192.0f;
      //*/

      //std::cout << tc << std::endl; 
      _texCoords.push_back(glm::vec2((float)tc.x, (float)tc.y));
    }
  }
  else
  {
    _texture = appearanceManager.getTexture( id );
    if ( !_texture ) _texture = dynamic_cast< Texture * >( defAppearance );
  }
}

void Polygon::addRing( LinearRing* ring )
{
  if ( ring->isExterior() ) _exteriorRing = ring;
  else _interiorRings.push_back( ring );
}
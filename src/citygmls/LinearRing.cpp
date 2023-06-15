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
#include <limits>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include "LinearRing.h"
#include "Envelope.h"

LinearRing::LinearRing( const std::string& id, bool isExterior )
    : Object( id ), _exterior( isExterior )
{}

bool LinearRing::isExterior() const
{
    return _exterior;
}

unsigned int LinearRing::size() const
{
    return (unsigned int)_vertices.size();
}

const std::vector<glm::highp_dvec3>& LinearRing::getVertices() const
{
    return _vertices;
}

void LinearRing::addVertex( const glm::highp_dvec3& v )
{
    _vertices.push_back( v );
}

// Return the envelope (ie. the bounding box) of the object
const Envelope& LinearRing::getEnvelope() const
{
    return _envelope;
}

std::vector<glm::highp_dvec3>& LinearRing::getVertices()
{
    return _vertices;
}

glm::highp_dvec3 LinearRing::computeNormal() const
{
    unsigned int len = size();
    if ( len < 3 ) return glm::highp_dvec3();

    // Tampieri, F. 1992. Newell's method for computing the plane equation of a polygon. In Graphics Gems III, pp.231-232.
    glm::highp_dvec3 n( 0., 0., 0. );
    for ( unsigned int i = 0; i < len; i++ )
    {
        const glm::highp_dvec3& current = _vertices[i];
        const glm::highp_dvec3& next = _vertices[ ( i + 1 ) % len];

        n.x += ( current.y - next.y ) * ( current.z + next.z );
        n.y += ( current.z - next.z ) * ( current.x + next.x );
        n.z += ( current.x - next.x ) * ( current.y + next.y );
    }
    return glm::normalize(n);
}

void LinearRing::finish( TexCoords* texCoords )
{
    // Remove last vertex which is supposed to be the same as the first one

   unsigned int len = (unsigned int)_vertices.size();
   if ( len < 2 ) return;

   float lengthSquare(glm::length2(_vertices[0] - _vertices[len - 1]));
   if (lengthSquare <= std::numeric_limits<double>::epsilon())
      _vertices.erase( _vertices.begin() + len - 1 );
}
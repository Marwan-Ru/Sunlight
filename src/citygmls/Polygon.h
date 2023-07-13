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

#pragma once

#include <vector>

#include <maths/Vector3.h>
#include <citygmls/Object.h>
#include "citygmls/Envelope.h"

#include "CityGmlTypes.h"

class Geometry;
class Appearance;
class AppearanceManager;
class Material;
class Texture;
class LinearRing;

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
#pragma warning(disable: 4251) // export problem on STL members
#endif

class Polygon : public Object
{
    friend class CityGMLHandler;
    friend class Geometry;
    friend class Tesseletor;
    friend class CityModel;
public:
    enum AppearanceSide {
        FRONT = 0,
        BACK,
        _NUMBER_OF_SIDES
    };

    Polygon( const std::string& id );

    virtual ~Polygon() override;

    //Polygon* Clone();

    // Get the vertices
    const std::vector<TVec3d>& getVertices() const;

    // Get the indices
    const std::vector<unsigned int>& getIndices() const;

    // Get the normals
    const std::vector<TVec3f>& getNormals() const;

    // Get texture coordinates
    TexCoords& getTexCoords();
    const TexCoords& getTexCoords() const;

    // Get the appearance
    const Appearance* getAppearance() const; // Deprecated! Use getMaterial and getTexture instead
    const Material* getMaterial() const;
    const Texture* getTexture() const;
    const Material* getMaterialFront() const;
    const Material* getMaterialBack() const;

    const std::vector<LinearRing*>& getInteriorRings() const;
    std::vector<LinearRing*>& getInteriorRings();

    const LinearRing* getExteriorRing() const;
    LinearRing* getExteriorRing();

    // Return the envelope (ie. the bounding box) of the object
    const Envelope& getEnvelope() const;

//	protected:
    void finish( AppearanceManager&, bool doTesselate );
    void finish( AppearanceManager&, Appearance*,  bool doTesselate );

    void addRing( LinearRing* );

    void tesselate( AppearanceManager &, const TVec3d& );
    void mergeRings( AppearanceManager & );
    void clearRings();

    TVec3d computeNormal();

    bool merge( Polygon* );

protected:
    std::vector<TVec3d> _vertices;
    std::vector<TVec3f> _normals;
    std::vector<unsigned int> _indices;

    Appearance* _appearance;
    Material* _materials[ _NUMBER_OF_SIDES ];
    Texture* _texture;

    TexCoords _texCoords;

    LinearRing* _exteriorRing;
    std::vector<LinearRing*> _interiorRings;

    bool _negNormal;

    Geometry *_geometry;

    Envelope _envelope;
};

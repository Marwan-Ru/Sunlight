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

#include "Object.h"
#include "CityGmlTypes.h"
#include "Envelope.h"
#include <glm/vec3.hpp>

#include <vector>
#include <string>

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
#pragma warning(disable: 4251) // export problem on STL members
#endif

class LinearRing : public Object
{
    friend class CityGMLHandler;
    friend class Polygon;
public:
    LinearRing( const std::string& id, bool isExterior );

    bool isExterior() const;

    unsigned int size() const;

    const std::vector<glm::highp_dvec3>& getVertices() const;

    void addVertex( const glm::highp_dvec3& v );

    glm::highp_dvec3 computeNormal() const;

    // Return the envelope (ie. the bounding box) of the object
    const Envelope& getEnvelope() const;

    std::vector<glm::highp_dvec3>& getVertices();

    void finish( TexCoords* );

protected:
    bool _exterior;

    std::vector<glm::highp_dvec3> _vertices;

    Envelope _envelope;
};
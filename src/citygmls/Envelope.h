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

#include <ostream>

#include <glm/vec3.hpp>

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
#pragma warning(disable: 4251) // export problem on STL members
#endif

class Envelope
{
    friend class CityGMLHandler;
public:
    Envelope();
    Envelope(const glm::highp_dvec3& lowerBound, const glm::highp_dvec3& upperBound);

    const glm::highp_dvec3& getLowerBound() const;
    const glm::highp_dvec3& getUpperBound() const;

    void merge(const Envelope& e);

    void merge(const glm::highp_dvec3& p);

protected:
    glm::highp_dvec3 _lowerBound;
    glm::highp_dvec3 _upperBound;
};

std::ostream& operator<<( std::ostream&, const Envelope& );
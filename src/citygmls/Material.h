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

#include "Appearance.h"
#include <maths/Vectors.h>

class Material : virtual public Appearance
{
    friend class CityGMLHandler;
public:
    Material( const std::string& id );

    TVec3f getDiffuse() const;
    TVec3f getEmissive() const;
    TVec3f getSpecular() const;
    float getAmbientIntensity() const;
    float getShininess() const;
    float getTransparency() const;

protected:
    TVec3f _diffuse;
    TVec3f _emissive;
    TVec3f _specular;
    float _ambientIntensity;
    float _shininess;
    float _transparency;
};
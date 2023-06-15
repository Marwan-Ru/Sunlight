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

#include "Material.h"

Material::Material( const std::string& id )
    : Appearance( id, "Material" ), _ambientIntensity( 0.f ), _shininess( 0.f ), _transparency( 0.f )
{}

glm::vec3 Material::getDiffuse() const
{
    return _diffuse;
}

glm::vec3 Material::getEmissive() const
{
    return _emissive;
}

glm::vec3 Material::getSpecular() const
{
    return _specular;
}

float Material::getAmbientIntensity() const
{
    return _ambientIntensity;
}

float Material::getShininess() const
{
    return _shininess;
}

float Material::getTransparency() const
{
    return _transparency;
}
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

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
# pragma warning(disable:4251) // gdal-1.11.4 internals (cpl_string.h) when
#endif 

class Appearance : public Object
{
    friend class CityGMLHandler;
public:
    Appearance( const std::string& id, const std::string& typeString );

    virtual ~Appearance();

    std::string getType() const;
    bool getIsFront() const;

    virtual std::string toString() const;

protected:
    std::string _typeString;
    bool _isFront;
};
// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <vector>
#include <memory>

#include <maths/Vector3.h>
#include <maths/Vector2.h>

/**
*	@brief A simple ray
*/
struct Ray
{
    Ray() = default;

    /**
    *	@brief Build a new ray
    *	@param _origin Origin of the ray
    *	@param _direction Direction of the ray
    */
    Ray(const TVec3d& _origin, const TVec3d& _direction) :
       origin(_origin), direction(_direction)
    {
    }

    /**
     * @brief Origin of the ray
    */
    TVec3d origin;

    /**
     * @brief Direction of the ray
    */
    TVec3d direction;
};

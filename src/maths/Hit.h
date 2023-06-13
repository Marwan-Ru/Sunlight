// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include "Vectors.h"
#include "Triangle.h"
#include "Ray.h"

/**
*	@brief An intersection between a ray and a triangle
*/
struct Hit
{
    /**
    *	@brief Create a new Hit
    */
    Hit();

    Ray ray;///< The ray that hits
    bool intersect;///< If yes or no the ray hits the triangle
    float distance;///< Distance from the origin of the ray to the hit position
    float parameter;///< T0
    float triangleBary[3];///< ?
    TVec3d point;///< Hit position
    Triangle triangle;///< Triangle that has been hit
};
// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <maths/Vector3.h>

#include "Triangle.h"
#include "Ray.h"

/**
*	@brief An intersection between a ray and a triangle
*/
struct RayHit
{
   RayHit() = default;

   RayHit(const Ray& ray, const TVec3d& impactPoint, const Triangle& triangle) :
      m_ray(ray), m_impactPoint(impactPoint), m_triangle(triangle)
   {
   }

    Ray m_ray;///< The ray that hits
    TVec3d m_impactPoint;///< Hit position
    Triangle m_triangle;///< Triangle that has been hit
};

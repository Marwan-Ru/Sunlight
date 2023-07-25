// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <maths/Vector3.h>

#include "Triangle.h"
#include "Ray.h"

/**
*	@brief Response to an intersection between a ray and a triangle
*/
struct RayHit
{
   RayHit() = default;

   RayHit(const TVec3d& _impactPoint, const Triangle& _triangle, float _distance) :
      impactPoint(_impactPoint), triangle(_triangle), distance(_distance)
   {
   }

   /**
   * @brief Impact position of the ray
   */
   TVec3d impactPoint;

   /**
   * @brief Hit triangle by the ray
   */
   Triangle triangle;

   /**
   * @brief Precompute distance between ray origin and impact point
   */
   float distance;
};

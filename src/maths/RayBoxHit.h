// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <vector>

#include "AABB.h"
#include "Ray.h"

/**
*	@brief Response to an intersection between a ray and a bounding box
*/
struct RayBoxHit
{
   RayBoxHit() = default;

   RayBoxHit(const AABB& box, float distance)
      : box(box), distance(distance)
   {
   }

   /**
   * @brief Bounding box hit by the ray
   */
    AABB box;

    /**
   * @brief Precompute distance between ray origin and impact point
   */
    float distance;
};

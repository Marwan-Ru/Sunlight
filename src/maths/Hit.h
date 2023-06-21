// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <glm/vec3.hpp>

#include "Triangle.h"
#include "Ray.h"

/**
*	@brief An intersection between a ray and a triangle
*/
struct Hit
{
    Ray ray;///< The ray that hits
    float distance;///< Distance from the origin of the ray to the hit position
    glm::highp_dvec3 impactPoint;///< Hit position
    Triangle triangle;///< Triangle that has been hit
};

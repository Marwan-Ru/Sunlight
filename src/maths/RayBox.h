// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <vector>

#include "AABB.h"
#include "Ray.h"

/**
*	@brief A hit between a ray and a bounding box
*/
struct RayBoxHit
{
    AABB box;///< Box hitted
    float minDistance;///< Distance between the ray origin and the bounding box
    std::vector<unsigned int> ListRays; //Contient la liste des rayons intersectant cette box.
};

bool operator<(const RayBoxHit& a, const RayBoxHit& b)
{
   return a.minDistance < b.minDistance;
}

/**
 * @brief Ray - Bounding Boxes related methods and members
 */
class RayBox : public Ray
{

public:

   RayBox() = default;

    /**
    *	@brief Build a new rayBox
    *	@param _origin Origin of the rayBox
    *	@param _direction Direction of the rayBox
    */
    RayBox(TVec3d _origin, TVec3d _direction) :
       Ray(_origin, _direction), boxes(std::vector<RayBoxHit>())
    {
    }

    std::vector<RayBoxHit> boxes;///< List of boxes that this ray go through
};

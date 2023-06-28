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

bool operator<(const RayBoxHit& a, const RayBoxHit& b);


/**
 * @brief Ray - Bounding Boxes related methods and members
 */
class RayBox : public Ray
{

public:

    /**
    *	@brief Build a new rayBox
    *	@param ori Origin of the rayBox
    *	@param dir Direction of the rayBox
    * @param id oh the rayBox
    */
    RayBox(glm::highp_dvec3 ori = glm::highp_dvec3(0.0, 0.0, 0.0), glm::highp_dvec3 dir = glm::highp_dvec3(1.0, 1.0, 1.0), int id = -1);

    /**
    *	@brief To know if the ray instersects a given box
    *	@param box Box to intersect
    *	@param hitt0 t0 at when ray first hits the box
    *	@param hitt1 t1 at when ray first leaves the box
    *	@return True if this ray intersect with the box
    */
    bool Intersect(AABB box, float* hitt0, float* hitt1);

    std::vector<RayBoxHit> boxes;///< List of boxes that this ray go through

};



/**
*	@brief A collection of RayBox
*/
struct RayBoxCollection
{
    /**
    *	@brief Build a new collection
    */
    RayBoxCollection(std::vector<RayBox*> raysBoxes = std::vector<RayBox*>());

    ~RayBoxCollection();

    std::vector<RayBox*> raysBB;///< The rays of the collection
};

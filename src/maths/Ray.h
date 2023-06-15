// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Hit;
struct RayCollection;
struct ViewPoint;
struct Triangle;

/**
*	@brief A simple ray
*/
struct Ray
{
    /**
    *	@brief Build a new ray
    *	@param ori Origin of the ray
    *	@param dir Direction of the ray
    *   @param Id oh the ray
    */
    Ray(glm::highp_dvec3 ori = glm::highp_dvec3(0.0, 0.0, 0.0), glm::highp_dvec3 dir = glm::highp_dvec3(1.0, 1.0, 1.0), int id = -1);

    /**
    *	@brief To know if the ray instersect a given triangle
    *	@param triangle The triangle to intersect
    *	@param hit Information about the intersection will be stored in it
    *	@return True if this ray intersect with the triangle, false otherwise
    */
    bool Intersect(Triangle* triangle, Hit* hit = nullptr);

    int id;///< Id of the ray
    glm::highp_dvec2 fragCoord;///< Fragment coordinate of the ray
    glm::highp_dvec3 origin;///< Origin of the ray
    glm::highp_dvec3 direction;///< Direction of the ray
    glm::highp_dvec3 inv_direction;///< inv Direction of the ray
    int sign[3];
};

/**
*	@brief A collection of rays
*/
struct RayCollection
{
    /**
    *	@brief Build a new collection
    */
    RayCollection(std::vector<Ray*> rays = std::vector<Ray*>());

    ~RayCollection();

    std::vector<Ray*> rays;///< Rays of the collection
};
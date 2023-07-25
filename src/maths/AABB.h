// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <string>
#include <vector>
#include <optional>

#include <maths/Vector3.h>

struct Ray;
struct RayBoxHit;

/**
*	@brief An axis aligned bounding box
*/
struct AABB
{
   AABB() = default;
   AABB(const TVec3d& _min, const TVec3d& _max, const std::string& id, const std::string& tileName);

   /**
    * @brief Get the triangle id that can be used to identify one
    *         triangle in a triangle soup.
    * @return
   */
   const std::string& getId() const;

   /**
    * @brief Get tile name containing the current triangle.
    * @return
   */
   const std::string& getTileName() const;

   /**
   *	@brief Check intersection between a ray and a bounding box.
   *	@param ray
   *	@return An optional RayHit containing intersection information.
   */
   std::optional<RayBoxHit> doesIntersect(const Ray& ray) const;

   /**
   * @brief Min and Max point of the box
   */
   TVec3d min, max;

   /**
   * @brief Bounding box identifier.
   */
   std::string m_id;

   /**
   * @brief Tile name corresponding to the bounding box
   */
   std::string m_tileName;
};
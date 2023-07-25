// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "AABB.h"
#include "Ray.h"
#include "RayBoxHit.h"

AABB::AABB(const TVec3d& _min, const TVec3d& _max, const std::string& id, const std::string& tileName) :
      min(_min), max(_max), m_id(id), m_tileName(tileName)
{
}

const std::string& AABB::getId() const
{
   return m_id;
}

const std::string& AABB::getTileName() const
{
   return m_tileName;
}

//Ray aabb intersection, from pbrt-v2
//License : http://www.pbrt.org/LICENSE.txt
std::optional<RayBoxHit> AABB::doesIntersect(const Ray& ray) const
{
   double t0 = 0, t1 = std::numeric_limits<double>::max();
   for (int i = 0; i < 3; ++i) {
      // Update interval for _i_th bounding box slab
      double invRayDir = 1.0 / ray.direction[i];
      double tNear = (min[i] - ray.origin[i]) * invRayDir;
      double tFar = (max[i] - ray.origin[i]) * invRayDir;

      // Update parametric interval from slab intersection $t$s
      if (tNear > tFar) std::swap(tNear, tFar);
      t0 = tNear > t0 ? tNear : t0;
      t1 = tFar < t1 ? tFar : t1;
      if (t0 > t1) return {};
   }

   // Smallest hit distance
   double distance = t0;

   return RayBoxHit(*this, distance);
}

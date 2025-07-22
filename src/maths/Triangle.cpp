// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <utility>
#include <vector>

#include "Triangle.h"
#include "RayHit.h"
#include "Ray.h"

Triangle::Triangle(TVec3d a, TVec3d b, TVec3d c)
{
   this->a = a;
   this->b = b;
   this->c = c;
   this->calculateNormal();
}

Triangle::Triangle(const TVec3d& _a, const TVec3d& _b, const TVec3d& _c, std::string  triangleId, std::string  tileName)
   : a(_a), b(_b), c(_c), m_id(std::move(triangleId)), m_tileName(std::move(tileName))
{
   this->calculateNormal();
}

Triangle::Triangle(const TVec3d& _a, const TVec3d& _b, const TVec3d& _c, std::string  triangleId, std::string  tileName, const TVec3d& _n)
   : a(_a), b(_b), c(_c), n(_n), m_id(std::move(triangleId)), m_tileName(std::move(tileName))
{
}

const std::string& Triangle::getId() const
{
   return m_id;
}

const std::string& Triangle::getTileName() const
{
   return m_tileName;
}

TVec3d Triangle::getNormal() const
{
   return this->n;
}

TVec3d Triangle::getBarycenter() const
{
   return (a + b + c) / 3.0;
}

/**
 * @brief Ray - Triangle intersection method, checks if the given ray intersects with this triangle. Does not take into account normals
 * @param ray A Ray object with which to do the intersection test
 * @return std::optional<RayHit> object, if it contains a value there was an intersection
 */
std::optional<RayHit> Triangle::doesIntersect(const Ray& ray) const
{
   constexpr float epsilon = std::numeric_limits<float>::epsilon();

   const TVec3 edge1 = b - a;
   const TVec3 edge2 = c - a;
   const TVec3 ray_cross_e2 = ray.direction.cross(edge2);
   const float det = edge1.dot(ray_cross_e2);

   if (det > -epsilon && det < epsilon)
      return {};    // This ray is parallel to this triangle.

   float inv_det = 1.0 / det;
   TVec3 s = ray.origin - a;
   float u = inv_det * s.dot(ray_cross_e2);

   if ((u < 0 && abs(u) > epsilon) || (u > 1 && abs(u-1) > epsilon))
      return {};

   TVec3 s_cross_e1 = s.cross(edge1);
   float v = inv_det * ray.direction.dot(s_cross_e1);

   if ((v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon))
      return {};

   // At this stage we can compute t to find out where the intersection point is on the line.
   float t = inv_det * edge2.dot(s_cross_e1);

   if (t > epsilon) // ray intersection
   {
      TVec3d impactPoint(ray.origin + ray.direction * inv_det * t);

      float distance ((ray.direction * t * inv_det).length());

      return  RayHit(impactPoint, *this, distance);
   }
   else // This means that there is a line intersection but not a ray intersection.
      return  {};

}

void Triangle::calculateNormal() {
   auto x = b - a;
   auto y = c - a;
   auto normal = x.cross(y);

   this->n = normal.normalize();
}

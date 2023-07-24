// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <vector>

#include "Triangle.h"
#include "RayHit.h"
#include "Ray.h"

Triangle::Triangle(TVec3d a, TVec3d b, TVec3d c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

Triangle::Triangle(const TVec3d& _a, const TVec3d& _b, const TVec3d& _c, const std::string& triangleId, const std::string& tileName)
   : a(_a), b(_b), c(_c), m_id(triangleId), m_tileName(tileName)
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
   auto x = b - a;
   auto y = c - a;
   auto normal = x.cross(y);

   return normal.normalize();
}

TVec3d Triangle::getBarycenter() const
{
   return (a + b + c) / 3.0;
}

//Ray triangle intersection, from geometric tools engine
//License : http://www.boost.org/LICENSE_1_0.txt
std::optional<RayHit> Triangle::doesIntersect(const Ray& ray) const
{
   // Compute the offset origin, edges, and normal.
   TVec3d diff = ray.origin - a;
   TVec3d normal = getNormal();

   TVec3d edge1 = b - a;
   TVec3d edge2 = c - a;

   // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
   // E1 = edge1, E2 = edge2, N = Cross(E1,E2)) by
   //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
   //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
   //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
   double DdN = ray.direction.dot(normal);
   double sign;
   if (0.0 < DdN)
   {
      sign = 1.0;
   }
   else if (DdN < 0.0)
   {
      sign = -1.0;
      DdN = -DdN;
   }
   else
   {
      // Ray and triangle are parallel, call it a "no intersection"
      // even if the ray does intersect.
      return {};
   }

   double DdQxE2 = sign * ray.direction.dot(diff.cross(edge2));
   if (0.0 <= DdQxE2)
   {
      double DdE1xQ = sign * ray.direction.dot(edge1.cross(diff));
      if (0.0 <= DdE1xQ)
      {
         if (DdQxE2 + DdE1xQ <= DdN)
         {
            // Line intersects triangle, check whether ray does.
            double QdN = -sign * diff.dot(normal);
            if (0.0 <= QdN)
            {
               // Ray intersects triangle.
               double inv = ((float)1) / DdN;
               TVec3d impactPoint(ray.origin + inv * QdN * ray.direction);

               float distance ((ray.direction * QdN * inv).length());

               return RayHit(impactPoint, *this, distance);
            }
            // else: t < 0, no intersection
         }
         // else: b1+b2 > 1, no intersection
      }
      // else: b2 < 0, no intersection
   }
   // else: b1 < 0, no intersection

   return {};
}

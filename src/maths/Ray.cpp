// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "Ray.h"
#include "RayHit.h"
#include "Triangle.h"

//Ray

Ray::Ray(TVec3d ori, TVec3d dir, int id)
{
    this->id = id;
    this->origin = ori;
    this->direction = dir;
    inv_direction = TVec3d(1 / dir.x, 1 / dir.y, 1 / dir.z);
    sign[0] = (inv_direction.x < 0);
    sign[1] = (inv_direction.y < 0);
    sign[2] = (inv_direction.z < 0);
    fragCoord.x = -1;
    fragCoord.y = -1;
}

float DotCross(TVec3d v0, TVec3d v1, TVec3d v2)
{
   return v0.dot(v1.cross(v2));
}

//Ray triangle intersection, from geometric tools engine
//License : http://www.boost.org/LICENSE_1_0.txt
bool Ray::Intersect(std::shared_ptr<Triangle> triangle, RayHit* hit)
{
   RayHit tempHit;

   // Compute the offset origin, edges, and normal.
   TVec3d diff = origin - triangle->a;
   TVec3d edge1 = triangle->b - triangle->a;
   TVec3d edge2 = triangle->c - triangle->a;
   TVec3d normal = edge1.cross(edge2);

   // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
   // E1 = edge1, E2 = edge2, N = Cross(E1,E2)) by
   //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
   //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
   //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
   double DdN = direction.dot(normal);
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
      return false;
   }

   double DdQxE2 = sign * DotCross(direction, diff, edge2);
   if (0.0 <= DdQxE2)
   {
      double DdE1xQ = sign * DotCross(direction, edge1, diff);
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
               tempHit.impactPoint = origin + inv * QdN * direction;

               tempHit.triangle = *triangle;
               tempHit.ray = (*this);

               if (hit != nullptr)
               {
                  *hit = tempHit;
               }
               return true;
            }
            // else: t < 0, no intersection
         }
         // else: b1+b2 > 1, no intersection
      }
      // else: b2 < 0, no intersection
   }
   // else: b1 < 0, no intersection

   return false;
}

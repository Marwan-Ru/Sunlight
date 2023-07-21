#include <iostream>

#include "API.h"
#include <maths/Ray.h>
#include <maths/RayHit.h>
#include <maths/Triangle.h>
#include <maths/AABB.h>
#include <maths/RayTracing.h>

bool isFacingTheSun(const Triangle& triangle, const TVec3d& sunDirection)
{
   return 0.0 <= triangle.getNormal().dot(sunDirection);
}

Ray constructRay(const Triangle& triangle, const TVec3d& sunDirection)
{
   const float OFFSET(0.01f);

   TVec3d origin(triangle.getBarycenter());
   
   // Add an offset for raytracing. Without this offset, origin of the ray might be behind the barycenter,
   // which will result in a collision between the ray its origin triangle
   TVec3d direction(origin + sunDirection * OFFSET);

   return Ray(origin, direction);
}

std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<AABB>& boundingBoxes)
{
   auto result = std::vector<RayHit>();

   for (const auto& boundingBoxes : boundingBoxes)
   {
      auto rayHit(boundingBoxes.doesIntersect(ray));
      if (!rayHit.has_value())
         continue;

      result.push_back(rayHit.value());
   }

   // Sort by distance (from near to far)
   std::sort(result.begin(), result.end(), [](const RayHit& a, const RayHit& b)
   {
      return a.distance < b.distance;
   });

   return result;
}

std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<Triangle>& triangleSoup)
{
   auto result = std::vector<RayHit>();

   for (const auto& triangle : triangleSoup)
   {
      auto rayHit(triangle.doesIntersect(ray));
      if (!rayHit.has_value())
         continue;

      result.push_back(rayHit.value());
   }

   // Sort by distance (from near to far)
   std::sort(result.begin(), result.end(), [](const RayHit& a, const RayHit& b)
   {
      return a.distance < b.distance;
   });

   return result;
}

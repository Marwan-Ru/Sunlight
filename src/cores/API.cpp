#include <iostream>

#include "API.h"
#include <maths/Ray.h>
#include <maths/RayHit.h>
#include <maths/Triangle.h>
#include <maths/AABB.h>

bool isFacingTheSun(const Triangle& triangle, const TVec3d& sunPosition)
{
   return true;
}

std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<AABB>& boundingBoxes)
{
   std::cout << "Check intersection with a bounding box vector containing " << boundingBoxes.size() << " AABB." << std::endl;
   return std::vector<RayHit>();
}

std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<Triangle>& triangleSoup)
{
   std::cout << "Check intersection with a triangle soup containing " << triangleSoup.size() << " triangles." << std::endl;
   return std::vector<RayHit>();
}

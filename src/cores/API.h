#pragma once

#include <vector>

#include <maths/Vector3.h>

struct Ray;
struct RayHit;
class Triangle;
class AABB;

bool isFacingTheSun(const Triangle& triangle, const TVec3d& sunPosition);

std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<AABB>& boundingBoxes);

std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<Triangle>& triangleSoup);

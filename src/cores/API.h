#pragma once

#include <vector>

#include <maths/Vector3.h>

struct Ray;
struct RayHit;
struct Triangle;
struct AABB;

/**
 * @brief Check that the normal of the given triangle is in the direction of the sun (aka facing the sun).
 * @param triangle Tested triangle.
 * @param sunDirection Sunlight direction.
 * @return A boolean corresponding to the triangle facing the sun or not.
*/
bool isFacingTheSun(const Triangle& triangle, const TVec3d& sunDirection);

/**
 * @brief Construct a ray from a triangle in direction of the sun.
 * @param triangle Origin of the created ray.
 * @param sunDirection Sun Direction of the ray.
 * @return a ray from the triangle to the sun.
*/
Ray constructRay(const Triangle& triangle, const TVec3d& sunDirection);

/**
 * @brief Check all intersection with a ray and a list of bounding boxes.
 *         It's sort the output list based on impact distance from near to far.
 * @param ray Ray used to check intersections.
 * @param boundingBoxes All boundings where we check an intersection.
 * @return A vector of RayHit containing the impacted bounding boxes sorted by impact distance (from near to far).
*/
std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<AABB>& boundingBoxes);

/**
 * @brief Check all intersection with a ray and a triangle soup
 *         It's sort the output list based on impact distance from near to far.
 * @param ray Ray used to check intersections.
 * @param triangleSoup All triangles where we check an intersection.
 * @return A vector of RayHit containing the impacted triangles sorted by impact distance (from near to far).
 */
std::vector<RayHit> checkIntersectionWith(const Ray& ray, const std::vector<Triangle>& triangleSoup);
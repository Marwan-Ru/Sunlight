//
// Created by marwan on 7/22/25.
//

#ifndef INTERSECTION_TEST_H
#define INTERSECTION_TEST_H
#include <iostream>
#include <string>

#include "maths/Triangle.h"
#include "maths/Ray.h"
#include "maths/RayBoxHit.h"
#include "maths/AABB.h"

inline void launchTest(const std::string& name, const bool res) {
    std::cout << "Testing " << name << " ... ";
    if(res) {
        std::cout << "OK" << std::endl;
    }else {
        std::cout << "FAILED" << std::endl;
    }
}

/**
 * @brief Checks if the ray intersects correctly with the face of a triangle (simple case, the ray intersects the centroid)
 * @return true if the ray intersects the triangle
 */
inline bool singleTriangleIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto t = Triangle(TVec3d(1, -1, -1), TVec3d(1, -1, 1), TVec3d(1, 1, -1));

    const std::optional<RayHit> res = t.doesIntersect(r);

    return res.has_value();
}

/**
 * @brief Checks that the ray does not intersect a triangle that it should not
 * @return true if the ray does not intersect the triangle
 */
inline bool singleTriangleNoIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto t = Triangle(TVec3d(1, -2, -2), TVec3d(1, -1, -1), TVec3d(1, -1, 1));

    const std::optional<RayHit> res = t.doesIntersect(r);

    return !res.has_value();
}

/**
 * @brief Checks that the ray does not intersect triangles that are behind its starting point
 * @return true if the ray does not intersect the triangle
 */
inline bool singleTriangleBackwardIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(-1, 0, 0));
    const auto t = Triangle(TVec3d(1, -1, -1), TVec3d(1, -1, 1), TVec3d(1, 1, -1));
    // Result should be not intersection since the triangle is behind the ray
    const std::optional<RayHit> res = t.doesIntersect(r);

    return !res.has_value();
}

/**
 * @brief Checks if the ray intersects correctly when it's right on a vertex
 * @return true if the ray intersects the triangle
 */
inline bool singleTriangleVertexIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto t = Triangle(TVec3d(1, 0, 0), TVec3d(1, 1, 0), TVec3d(1, 0, 1));

    const std::optional<RayHit> res = t.doesIntersect(r);

    return res.has_value();
}

/**
 * @brief Checks if the ray intersects correctly when it's right on an edge
 * @return true if the ray intersects the triangle
 */
inline bool singleTriangleEdgeIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto t = Triangle(TVec3d(1, 0, -1), TVec3d(1, 1, 0), TVec3d(1, 0, 1));

    const std::optional<RayHit> res = t.doesIntersect(r);

    return res.has_value();
}

/**
 * @brief Checks if the ray intersects correctly with a box that is right in front of it
 * @return true if the ray intersects the AABB
 */
inline bool AABBRayIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto box = AABB(TVec3d(1, -100, -100), TVec3d(10, 100, 100), "test", "testtile");

    const std::optional<RayBoxHit> res = box.doesIntersect(r);

    return res.has_value();
}

/**
 * @brief Checks if the ray intersects with a box that is placed behind its origin (It should not)
 * @return True if the ray does not intersect the AABB
 */
inline bool AABBRayNoIntersect() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto box = AABB(TVec3d(-10, -100, -100), TVec3d(-1, 100, 100), "test", "testtile");

    const std::optional<RayBoxHit> res = box.doesIntersect(r);

    return !res.has_value();
}

/**
 * @brief Checks if the ray intersects the box, when the origin is inside of the box
 * @return True if the ray intersects the box
 */
inline bool AABBRayInside() {
    const auto r = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0));
    const auto box = AABB(TVec3d(-10, -100, -100), TVec3d(10, 100, 100), "test", "testtile");

    const std::optional<RayBoxHit> res = box.doesIntersect(r);

    return res.has_value();
}

/**
 * @brief Checks if the ray intersects the box, when the origin is right on the boxes face
 * @return True if the ray intersects the box
 */
inline bool AABBRayOnFace() {
    const auto r1 = Ray(TVec3d(0, 0, 0), TVec3d(1, 0, 0)); // Facing inside
    const auto r2 = Ray(TVec3d(0, 0, 0), TVec3d(-1, 0, 0)); // Facing outside
    const auto box = AABB(TVec3d(0, -100, -100), TVec3d(10, 100, 100), "test", "testtile");

    const std::optional<RayBoxHit> res1 = box.doesIntersect(r1);
    const std::optional<RayBoxHit> res2 = box.doesIntersect(r2);

    return res1.has_value() && res2.has_value();
}

inline bool AABBRayOnVertice() {
    const auto r1 = Ray(TVec3d(0, -100, -100), TVec3d(1, 0, 0));
    const auto r2 = Ray(TVec3d(10, 100, 100), TVec3d(-1, 0, 0));
    const auto box = AABB(TVec3d(0, -100, -100), TVec3d(10, 100, 100), "test", "testtile");

    const std::optional<RayBoxHit> res1 = box.doesIntersect(r1);
    const std::optional<RayBoxHit> res2 = box.doesIntersect(r2);

    return res1.has_value() && res2.has_value();
}

/**
 * Runs the various tests in the test suite
 * @todo Implement a real test framework to run the tests
 */
inline void runTests() {
    std::cout << "--- RAY-TRIANGLE INTERSECTION ---" << std::endl;
    launchTest("Single ray-triangle", singleTriangleIntersect());
    launchTest("Single ray-triangle no intersect", singleTriangleNoIntersect());
    launchTest("Single ray-triangle backward", singleTriangleBackwardIntersect());
    launchTest("Single ray-triangle vertex case", singleTriangleVertexIntersect());
    launchTest("Single ray-triangle edge case", singleTriangleEdgeIntersect());
    std::cout << "----- RAY-AABB INTERSECTION -----" << std::endl;
    launchTest("Single ray-AABB", AABBRayIntersect());
    launchTest("Single ray-AABB no intersect", AABBRayNoIntersect());
    launchTest("Single ray-AABB inside case", AABBRayInside());
    launchTest("Single ray-AABB face case", AABBRayOnFace());
    launchTest("Single ray-AABB vertex case", AABBRayOnVertice());
}

#endif //INTERSECTION_TEST_H

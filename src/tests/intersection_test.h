//
// Created by marwan on 7/22/25.
//

#ifndef INTERSECTION_TEST_H
#define INTERSECTION_TEST_H
#include <iostream>
#include <string>

#include "maths/Triangle.h"
#include "maths/Ray.h"

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

inline void runTests() {
    std::cout << "--- RAY-TRIANGLE INTERSECTION ---" << std::endl;
    launchTest("Single ray-triangle", singleTriangleIntersect());
    launchTest("Single ray-triangle no intersect", singleTriangleNoIntersect());
    launchTest("Single ray-triangle backward", singleTriangleBackwardIntersect());
    launchTest("Single ray-triangle vertex case", singleTriangleVertexIntersect());
    launchTest("Single ray-triangle edge case", singleTriangleEdgeIntersect());
}

#endif //INTERSECTION_TEST_H

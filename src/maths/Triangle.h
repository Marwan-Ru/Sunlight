// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>

#include <maths/Vector3.h>

struct Ray;
struct RayHit;

/**
*	@brief A triangle created from a citygml polygon
*/
struct Triangle
{
    /**
    *	@brief Build a new triangle
    *	@param a First point of the triangle
    *	@param b Second point of the triangle
    *	@param c Third point of the triangle
    */
    Triangle(TVec3d a = TVec3d(0.0, 0.0, 0.0), TVec3d b = TVec3d(0.0, 0.0, 0.0), TVec3d c = TVec3d(0.0, 0.0, 0.0));

    Triangle(const TVec3d& _a, const TVec3d& _b, const TVec3d& _c, std::string  triangleId, std::string  tileName);

    Triangle(const TVec3d &_a, const TVec3d &_b, const TVec3d &_c, std::string triangleId,
             std::string tileName, const TVec3d &_n);

    /**
     * @brief Get the triangle id that can be used to identify one
     *         triangle in a triangle soup.
     * @return
    */
    const std::string& getId() const;

    /**
     * @brief Get tile name containing the current triangle.
     * @return
    */
    const std::string& getTileName() const;

    /**
     * @brief Get normal of the current normal.
     * @return Normalized Vec3d corresponding to the triangle normal.
    */
    TVec3d getNormal() const;

    /**
     * @brief Get triangle barycenter.
     * @return Vec3D of triangle barycenter.
    */
    TVec3d getBarycenter() const;

    /**
    *	@brief Check intersection between a ray and a triangle.
    *	@param ray
    *	@return An optional RayHit containing intersection information.
    */
    std::optional<RayHit> doesIntersect(const Ray& ray) const;

    /**
     * @brief First Vertices of the triangle.
    */
    TVec3d a;

    /**
     * @brief Second Vertices of the triangle.
    */
    TVec3d b;

    /**
     * @brief Third Vertices of the triangle.
    */
    TVec3d c;

    /**
     * @brief Normal of the triangle.
     */
    TVec3d n;

    /**
     * @brief Triangle identifier.
    */
    std::string m_id;

    /**
     * @brief Tile name containing the actual triangle.
    */
    std::string m_tileName;

private:
    void calculateNormal();
};

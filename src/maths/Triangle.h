// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

#ifdef _MSC_VER
#pragma warning(disable: 4251) // VC++ DLL jejune complains on STL _Id member
#endif

enum CityObjectsType;

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
    Triangle(glm::highp_dvec3 a = glm::highp_dvec3(0.0, 0.0, 0.0), glm::highp_dvec3 b = glm::highp_dvec3(0.0, 0.0, 0.0), glm::highp_dvec3 c = glm::highp_dvec3(0.0, 0.0, 0.0));

    glm::highp_dvec3 GetNormal();

    glm::highp_dvec3 a; ///< First point of the triangle
    glm::highp_dvec3 b; ///< Second point of the triangle
    glm::highp_dvec3 c; ///< Third point of the triangle

    CityObjectsType objectType;
    CityObjectsType subObjectType;
    std::string objectId;
    std::string polygonId;
    std::string tileFile;
};

/**
*	A list of triangle
*/
struct TriangleList
{
    /**
    *	@brief Build a new collection of triangle
    */
    TriangleList(std::vector<Triangle*> triangles = std::vector<Triangle*>());
    ~TriangleList();

    std::vector<Triangle*> triangles;///< Triangles of the list
};

/**
*	@brief Build list of triangle from a CityGML building tile
*	@param tile CityGML tile from which we want the triangle list
*	@param viewpoint Data about the viewpoint we are rendering
*	@param objectType The type of cityobject to load
*   @param cityObjId The id of city object to load.
*          Default = "" (i.e. all cityobjects of the tile are loaded)
*   @param zMin A minimum z value of triangles to load
*          (if all vertices of the triangle are below this zMin value, triangle is not loaded)
*          Default = -10000.0
*	@return The list of triangle from the CityGML tile
*/
TriangleList* BuildTriangleList(
    const std::string& tilefilename,
    const CityObjectsType& objectType,
    const std::string& cityObjId = "",
    const double& zMin = -10000.0
);

// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <map>
#include <queue>
#include <vector>
#include <string>

#include <maths/Triangle.h>
#include <maths/AABB.h>
#include <maths/RayTracing.h>
#include <maths/RayBox.h>

class FileInfo;

/**
 * @brief Intersect rays with all boxes then sort the files intersected by intersection distance.
 * @param boxes Vector of files' bounding boxes to check intersection with.
 * @param rays Rays for intersection with bounding boxes testing.
 * @return a std::queue of RayboxHit sorted by intersection distance (min first).
*/
std::queue<RayBoxHit> intersectAndSortByDistance(const std::vector<AABB>& boxes, RayBoxCollection* rays);

/**
 * @brief Keep all the rays box that intersect with the current AABB and that do not already correspond
 * to an hour when the current triangle is in the shadow.
 * @param currentBBName Name of the current AABB.
 * @param allRayBoxes All rays for a triangle (corresponding to all the times of sunlight computation).
 * @param datetimeSunInfo Map holding sun and shadow information for current triangle.
 * @return A collection of RayBox intersecting with the current BB and not already intersecting with other geometries.
*/
RayBoxCollection* keepIntersectRaysBoxWith(const std::string& currentBBName, const RayBoxCollection* allRayBoxes, std::map<int, bool>& datetimeSunInfo);

/**
 * @brief Keep all the rays that intersect with the current AABB and that do not already correspond
 * to an hour when the current triangle is in the shadow.
 * @param currentBBName Name of the current AABB.
 * @param allRayBoxes All rays for a triangle (corresponding to all the times of sunlight computation).
 * @param datetimeSunInfo Map holding sun and shadow information for current triangle.
 * @return A collection of Rays intersecting with the current BB and not already intersecting with other geometries.
*/
RayCollection keepIntersectRaysWith(const std::string& currentBBName, const RayBoxCollection* allRayBoxes, std::map<int, bool>& datetimeSunInfo);

/**
 * @brief Load triangle list, run raytracing algorithm and update sunshine information.
 * @param filepath Path to file to check intersection with (current tile).
 * @param fileType Type of the file to check intersection with (current tile).
 * @param cityObjId Id of cityObject to test intersection with (Multi-resolution for _BATI).
 * @param rayColl Collection of Rays.
 * @param datetimeSunInfo Map holding sun and shadow result for current triangle that will be updated.
*/
void loadTriangleAndCheckIntersectionAndUpdateSunlightResult(const std::string& filepath, const CityObjectsType& fileType, const std::string& cityObjId, RayCollection& rayColl, std::map<int, bool>& datetimeSunInfo);

/**
 * @brief Compute sunlight on a given set of cityGML tiles on a predefined period.
 * @param fileDir Directory containing layer folder, that are containing tiles folder and tiles glm.
 * @param filenames All tiles cityGML in the file directory.
 * @param sunpathFile Full path to file describing sun path for a given year.
 * @param startDate Start date coded as integer.
 * @param endDate End date coded as integer.
 * @param outputDir Full path to output directory
*/
void computeSunlight(const std::string& fileDir, const std::vector<FileInfo>& filenames, const std::string& sunpathFile, const std::string& startDate, const std::string& endDate, const std::string& outputDir);

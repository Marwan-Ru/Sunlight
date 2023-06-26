// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <string>
#include <map>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/vec3.hpp>

struct Triangle;
class FileInfo;

/**
 * @brief This function creates SunlightOutput and _BATI, _MNT folders to follow this hierarchy :
 *
 *                              sOutputDir
 *                                  |
 *                            SunlightOutput
 *                                  |
 *                       ----------------------
 *                       |                    |
 *                     _BATI                _MNT
 *                       |                    |
 *                 -------------           -------------
 *                 |            |           |          |
 *         3670_10383  ... XXXX_XXXX   3670_10383  ... XXXX_XXXX
 *             |
 *      -------------------------
 *      |                       |
 * 2016-01-01.csv     ...   2016-31-12.csv
 * 
 * @param sOutputDir Path to output directory
*/
void createOutputFolders(const std::string& sOutputDir);

/**
 * @brief Creates folder for a given file which will hold its computed sunlight infos
 * @param file File to create folder for
 * @param sOutputDir Full path of the output directory
*/
void createFileFolder(FileInfo* file, const std::string& sOutputDir);

/**
 * @brief Export Sunlight results of a given triangle into a csv
 * @param sunInfo Map containing sunlight information (int : datetime, bool : sunny)
 * @param triangle Base triangle of sunlight computation
 * @param file Original file containing the triangle
 * @param iStartDate Start date of sunlight computation encoded as int
 * @param iEndDate End date of sunlight computation encoded as int
 * @param outputDir Full path to output directory
*/
void exportLightningToCSV(std::map<int, bool>& sunInfo, Triangle* triangle, FileInfo* file, int iStartDate, int iEndDate, std::string& outputDir);

/**
 * @brief Load a file containing the sun position for a year and filter by a given start and end date
 * @param sunpathFile Absolute path to the file
 * @param iStartDate Start date of sunlight computation encoded as int
 * @param iEndDate End date of sunlight computation encoded as int
 * @return 
*/
std::map<int, glm::highp_dvec3> loadSunpathFile(std::string sunpathFile, int iStartDate, int iEndDate);

/**
 * @brief Rotate a vector using a quaternion
 * @param vec Vector to rotate
 * @param q Rotation amount in vector
 * @return 
*/
glm::highp_dvec3 rotateVector(const glm::highp_dvec3& vec, const glm::dquat& q);

/**
 * @brief Compute sun rotation given an azimut and elevation angle
 * @param azimutAngleInRadians Azimut angle in radians of the sun
 * @param elevationAngleInRadians Elevation angle in radians of the sun
 * @return 
*/
glm::dquat getSunRotation(double azimutAngleInRadians, double elevationAngleInRadians);

/**
* @brief Compute sun direction given an azimut and elevation angle in radians
* @param azimutAngleInRadians Azimut angle in radians of the sun
* @param elevationAngleInRadians Elevation angle in radians of the sun
* @return Normalized direction towards the sun
*/
glm::highp_dvec3 computeDirectionTowardsTheSun(double azimutAngleInRadians, double elevationAngleInRadians);

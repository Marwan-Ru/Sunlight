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

///
/// \brief createOutputFolders Prepare folder layout hierarchy:
///
///                             sOutputDir
///                                 |
///                           SunlightOutput
///                                 |
///                      ----------------------
///                      |                    |
///                    _BATI                _MNT
///                      |                    |
///                -------------           -------------
///                |            |           |          |
///        3670_10383  ... XXXX_XXXX   3670_10383  ... XXXX_XXXX
///            |
///     -------------------------
///     |                       |
///2016-01-01.csv     ...   2016-31-12.csv
///
///
/// This function creates SunlightOutput and _BATI, _MNT folders.
///
/// \param sOutputDir Path to output directory
///
void createOutputFolders(const std::string& sOutputDir);

///
/// \brief createFileFolder Creates folder for a given file which will hold its computed sunlight infos
/// \param file File to create folder for
/// \param sOutputDir Full path to output directory
///
void createFileFolder(FileInfo* file, const std::string& sOutputDir);

///
/// \brief exportLightningToCSV Export Sunlight results of a given triangle into a csv.
/// \param sunInfo map containing sunlight information (int : datetime, bool : sunny)
/// \param t Triangle to export sunlight for
/// \param file File which holds the triangle
/// \param iStartDate Start date of sunlight computation encoded as int
/// \param iEndDate End date of sunlight computation encoded as int
/// \param outputDir Full path to output directory
///
void exportLightningToCSV(std::map<int, bool>& sunInfo, Triangle* t, FileInfo* file, int iStartDate, int iEndDate, std::string& outputDir);

///
/// \brief loadSunpathFile Loads file containing the path of the sun for a year
/// \param sunpathFile Full path to the file
/// \param iStartDate start date of sunlight computation encoded as int
/// \param iEndDate end date of sunlight computation encoded as int
/// \return
///
std::map<int, glm::highp_dvec3> loadSunpathFile(std::string sunpathFile, int iStartDate, int iEndDate);

/**
 * @brief Rotate a vector using a quaternion
 * @param vec Vector to rotate
 * @param q Rotation amount in vector
 * @return 
*/
glm::highp_dvec3 rotateVector(const glm::highp_dvec3& vec, const glm::dquat& q);

/// \brief Compute sun rotation given an azimut and elevation angle
/// \param azimutAngleInDegrees Azimut angle of the sun</param>
/// \param elevationAngleInDegrees Elevation angle of the sun
glm::dquat computeSunRotation(double azimutAngleInRadians, double elevationAngleInRadians);

/**
* @brief Compute sun direction given an azimut and elevation angle in radians
* @param azimutAngleInRadians Azimut angle in radians of the sun
* @param elevationAngleInRadians Elevation angle in radians of the sun
* @return Normalized direction towards the sun
*/
glm::highp_dvec3 computeDirectionTowardsTheSun(double azimutAngleInRadians, double elevationAngleInRadians);

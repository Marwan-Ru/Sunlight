// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <string>
#include <map>

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
void createFileFolder(const FileInfo& file, const std::string& sOutputDir);

/**
 * @brief Export Sunlight results of a given triangle into a csv
 * @param sunInfo Map containing sunlight information (int : datetime, bool : sunny)
 * @param triangle Base triangle of sunlight computation
 * @param file Original file containing the triangle
 * @param iStartDate Start date of sunlight computation encoded as int
 * @param iEndDate End date of sunlight computation encoded as int
 * @param outputDir Full path to output directory
*/
void exportLightningToCSV(std::map<int, bool>& sunInfo, const std::shared_ptr<Triangle>& triangle, const FileInfo& file, int iStartDate, int iEndDate, const std::string& outputDir);

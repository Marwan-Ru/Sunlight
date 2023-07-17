// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <string>
#include <map>

struct Triangle;

/**
 * @brief Export Sunlight results of a given triangle into a csv
 * @param sunInfo Map containing sunlight information (int : datetime, bool : sunny)
 * @param triangle Base triangle of sunlight computation
 * @param file Original file containing the triangle
 * @param iStartDate Start date of sunlight computation encoded as int
 * @param iEndDate End date of sunlight computation encoded as int
 * @param outputDir Full path to output directory
*/
void exportResult(std::map<int, bool>& sunInfo, const std::shared_ptr<Triangle>& triangle, const std::string& outputDir);

// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <fstream>

#include "SunlightCsvExporter.h"
#include <maths/Triangle.h>
#include <utils/DateTime.h>

void exportResult(std::map<int, bool>& sunInfo, const std::shared_ptr<Triangle>& triangle, const std::string& outputDir)
{
   for (const auto& [iDate, bTriangleInLight] : sunInfo)
   {
      std::string datetime = decodeDateTime(iDate);
      datetime.replace(datetime.find(':'), 1, " ");

      std::ofstream ofs;
      ofs.open(outputDir + "/Sunlight/" + triangle->getTileName() + "/" + datetime + ".obj", std::ofstream::app);

      ofs << datetime << ";" << triangle->getId() << ";" << std::to_string(bTriangleInLight) << std::endl;

      ofs.close();
   }
}

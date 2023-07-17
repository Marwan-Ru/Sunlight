#include <iostream>
#include <fstream>

#include "SunlightObjExporter.h"
#include <utils/DateTime.h>
#include <utils/Utils.h>


void SunlightObjExporter::exportResult(std::map<int, bool>& sunInfo, const std::shared_ptr<Triangle>& triangle, const std::string& outputDir)
{
   TVec3f shadowRgb(0, 0, 1);
   TVec3f lightRgb(1, 0, 0);

   for (const auto& [iDate, bTriangleInLight] : sunInfo)
   {
      std::string datetime = decodeDateTime(iDate);
      datetime.replace(datetime.find(':'), 1, " ");

      std::ofstream ofs;
      ofs.open(outputDir + "/Sunlight/" + triangle->getTileName() + "/" + datetime + ".obj", std::ofstream::app);

      TVec3f currentRgb = shadowRgb;
      if (bTriangleInLight)
      {
         currentRgb = lightRgb;
      }

      // Store vertex position and sunlight color as xyz rgb
      ofs << "v " << triangle->a.x << " " << triangle->a.y << " " << triangle->a.z << " " << currentRgb.r << " " << currentRgb.g << " " << currentRgb.b << std::endl;
      ofs << "v " << triangle->b.x << " " << triangle->b.y << " " << triangle->b.z << " " << currentRgb.r << " " << currentRgb.g << " " << currentRgb.b << std::endl;
      ofs << "v " << triangle->c.x << " " << triangle->c.y << " " << triangle->c.z << " " << currentRgb.r << " " << currentRgb.g << " " << currentRgb.b << std::endl;

      ofs << "f " << m_VertexIndex << " " << m_VertexIndex + 1 << " " << m_VertexIndex + 2 << std::endl;

      ofs.close();
   }

   m_VertexIndex += 3;
}

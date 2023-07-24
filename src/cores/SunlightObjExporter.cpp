#include <iostream>
#include <fstream>
#include <filesystem>

#include "SunlightObjExporter.h"
#include <maths/Triangle.h>
#include <utils/DateTime.h>

namespace fs = std::filesystem;

void SunlightObjExporter::exportResult(const std::string& dateStr, bool bTriangleInLight, const Triangle& triangle, const std::string& outputDir)
{
   TVec3f shadowRgb(0, 0, 1);
   TVec3f lightRgb(1, 0, 0);

   // Reformat date to follow file name conventions
   auto dateTime = dateStr;
   dateTime.replace(dateTime.find(":"), 1, "_");

   auto rootDirectory(outputDir + "/" + triangle.getTileName());
   fs::create_directories(rootDirectory);

   auto path(rootDirectory + "/" + dateTime + ".obj");
   std::ofstream ofs (path, std::ofstream::app);

   TVec3f currentRgb = shadowRgb;
   if (bTriangleInLight)
   {
      currentRgb = lightRgb;
   }

   // Store vertex position and sunlight color as xyz rgb
   ofs << "v " << triangle.a.x << " " << triangle.a.y << " " << triangle.a.z << " " << currentRgb.r << " " << currentRgb.g << " " << currentRgb.b << std::endl;
   ofs << "v " << triangle.b.x << " " << triangle.b.y << " " << triangle.b.z << " " << currentRgb.r << " " << currentRgb.g << " " << currentRgb.b << std::endl;
   ofs << "v " << triangle.c.x << " " << triangle.c.y << " " << triangle.c.z << " " << currentRgb.r << " " << currentRgb.g << " " << currentRgb.b << std::endl;

   ofs << "f " << m_VertexIndex << " " << m_VertexIndex + 1 << " " << m_VertexIndex + 2 << std::endl;

   ofs.close();

   m_VertexIndex += 3;
}


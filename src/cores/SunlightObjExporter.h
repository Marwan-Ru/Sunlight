#pragma once

#include <map>
#include <string>
#include <memory>

struct Triangle;

/**
 * @brief Sunlight result exporter in OBJ file.
*/
class SunlightObjExporter
{
private:
   int m_VertexIndex = 1;

public:
   /**
    * @brief Reset vertex count when a new file occur
   */
   void resetVertexCount();

   /**
    * @brief Create or replace an output directory
    * @param outputDir directory containing all obj exports.
   */
   void createOutputDirectory(const std::string& outputDir);

   /**
    * @brief Export sunlight result in an obj file
    * @param dateStr 3DUSE date format "2016-01-01:0800" for the 01/01/2016 at 08:00
    * @param bTriangleInLight sunlight result (in light nor not)
    * @param triangle 
    * @param outputDir 
   */
   void exportResult(const std::string& dateStr, bool bTriangleInLight, const Triangle& triangle, const std::string& outputDir);
};


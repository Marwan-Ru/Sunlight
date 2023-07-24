#pragma once

#include <map>
#include <string>
#include <memory>

struct Triangle;

class SunlightObjExporter
{
private:
   int m_VertexIndex = 1;

public:
   void exportResult(const std::string& dateStr, bool bTriangleInLight, const Triangle& triangle, const std::string& outputDir);
};


#pragma once

#include <map>
#include <string>

#include <maths/Triangle.h>

class SunlightObjExporter
{
private:
   int m_VertexIndex = 1;

public:
   void exportResult(std::map<int, bool>& sunInfo, const std::shared_ptr<Triangle>& triangle, const std::string& outputDir);
};


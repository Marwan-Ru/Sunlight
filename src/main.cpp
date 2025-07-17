#include "../cmake-build-release/_deps/spdlog-src/include/spdlog/spdlog.h"
#include "cores/API.h"
#include "parsers/SunEarthParser.h"
#include "maths/Ray.h"
#include "maths/RayHit.h"
#include "utils/rapidobj.hpp"

/**
 * @brief Builds sunlight triangles from rapidobj results
 * @param parsedObj A *triangulated* rapidobj::Result object
 * @return corresponding vector of sunlight Triangles
 */
std::vector<Triangle> getSunlightTriangleFromResults(rapidobj::Result& parsedObj) {
   // See https://github.com/guybrush77/rapidobj for a description of the structure of a rapidobj::Result object
   std::vector<Triangle> res;
   for (const auto& shape : parsedObj.shapes) {
      for (int i = 0; i < shape.mesh.indices.size(); i +=3) {
         // Getting each vertices index in the positions array
         const int p0 = shape.mesh.indices[i].position_index;
         const int p1 = shape.mesh.indices[i+1].position_index;
         const int p2 = shape.mesh.indices[i+2].position_index;

         // Getting each vertices positions, the array is flat and each vertice is composed of three floats
         auto a = TVec3d(parsedObj.attributes.positions[p0 * 3], parsedObj.attributes.positions[p0 * 3+1], parsedObj.attributes.positions[p0 * 3+2]);
         auto b = TVec3d(parsedObj.attributes.positions[p1 * 3], parsedObj.attributes.positions[p1 * 3+1], parsedObj.attributes.positions[p1 * 3+2]);
         auto c = TVec3d(parsedObj.attributes.positions[p2 * 3], parsedObj.attributes.positions[p2 * 3+1], parsedObj.attributes.positions[p2 * 3+2]);

         res.emplace_back(a, b, c, std::to_string(i/3), shape.name);
      }
   }
   return res;
}

int main()
{
   std::string objFilePath = "../datas/FakeData-1.obj";



   auto parsedObj = rapidobj::ParseFile(objFilePath);

   if (parsedObj.error) {
      spdlog::error(parsedObj.error.code.message());
      exit(EXIT_FAILURE);
   }

   if (rapidobj::Triangulate(parsedObj)) {
      spdlog::info("Triangulation ok");
   } else {
      spdlog::error("Could not triangulate mesh from {} file", objFilePath);
      exit(EXIT_FAILURE);
   }

   size_t num_triangles{};
   for (const auto& shape : parsedObj.shapes) {
      num_triangles += shape.mesh.num_face_vertices.size();
   }
   std::cout << "Shapes:    " << parsedObj.shapes.size() << std::endl;
   std::cout << "Materials: " << parsedObj.materials.size() << std::endl;
   std::cout << "Triangles: " << num_triangles << std::endl;

   std::vector<Triangle> sunTriangles = getSunlightTriangleFromResults(parsedObj);

   SunEarthToolsParser sun_parser;
   sun_parser.loadSunpathFile("../datas/AnnualSunPath_Lyon.csv", 403200, 403224);
   const std::vector<SunDatas> sundataList = sun_parser.getSunDatas();

   std::cout << sundataList.size() << std::endl;

   std::cout << "Number of sunlight triangles : " << sunTriangles.size() << std::endl;
   std::vector<RayHit> hits;
   for (const SunDatas& data : sundataList) {
       // Create all of my rays
      std::vector<Ray> rays;

      for (const Triangle& t : sunTriangles) {
         rays.push_back(constructRay(t, data.direction));
      }

      spdlog::info("Constructed {} rays", rays.size());

      for (Ray r : rays) {
         auto result = checkIntersectionWith(r, sunTriangles);
         hits.insert(hits.end(), result.begin(), result.end());
      }
   }
   spdlog::info("Total number of hits : {}", hits.size());
   for (const auto& hit : hits) {
      std::cout << hit.triangle.getId() << " ";
   }

   exit(EXIT_SUCCESS);
}

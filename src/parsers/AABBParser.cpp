#include <filesystem>
#include <iostream>
#include <fstream>
// Log in console
#include <spdlog/spdlog.h>

#include "AABBParser.h"
#include <maths/Triangle.h>
#include <citygmls/CityModel.h>
#include <citygmls/Tile.h>

namespace fs = std::filesystem;

#pragma region Loading AABB
std::vector<AABB> loadAABBFile(const std::string& path)
{
   std::vector<AABB> bSet;

   char line[256];

   std::ifstream ifs(path, std::ifstream::in);

   if (!ifs)
   {
      spdlog::error("Error with AABB file at {}", path);
      return bSet;
   }

   ifs.getline(line, 256);

   unsigned int count = atoi(line);

   for (unsigned int i = 0; i < count; i++)
   {
      AABB box;
      ifs.getline(line, 256);

      box.name = std::string(line);

      //To avoid problems with files from Windows used on UNIX (Windows uses '/r/n' as 'new line' escape sequence and Unix systems use '/n' only).
      //In order to erase '\r' character if present.
      if (!box.name.empty() && *box.name.rbegin() == '\r')
         box.name.erase(box.name.length() - 1, 1);

      double minx;
      double miny;
      double minz;
      double maxx;
      double maxy;
      double maxz;

      ifs.getline(line, 256); minx = atof(line);
      ifs.getline(line, 256); miny = atof(line);
      ifs.getline(line, 256); minz = atof(line);
      ifs.getline(line, 256); maxx = atof(line);
      ifs.getline(line, 256); maxy = atof(line);
      ifs.getline(line, 256); maxz = atof(line);

      if (minx < maxx && miny < maxy && minz <= maxz)
      {
         box.min = glm::highp_dvec3(minx, miny, minz);
         box.max = glm::highp_dvec3(maxx, maxy, maxz);
         bSet.push_back(box);
      }
   }

   ifs.close();

   return bSet;
}

std::optional<AABBCollection> loadLayersAABBs(const std::string& layerDirectory)
{
   //Check if our bounding box files do exists
   fs::path pathToLayerDirectory(layerDirectory);
   if (!fs::exists(pathToLayerDirectory))
   {
      spdlog::error("Error, files does not exists.");
      return {};
   }

   std::vector<AABB> buildingBoundingBoxes;
   std::vector<AABB> groundBoundingBoxes;

   for (const auto& layerFile : fs::directory_iterator(pathToLayerDirectory))
   {
      if (!layerFile.is_regular_file())
      {
         continue;
      }

      const std::string fileName (layerFile.path().filename().string());

      if (fileName == "_BATI_AABB.dat")
      {
         buildingBoundingBoxes = loadAABBFile(layerFile.path().string());
      }
      if (fileName == "_MNT_AABB.dat")
      {
         groundBoundingBoxes = loadAABBFile(layerFile.path().string());
      }
   }

   AABBCollection collection;
   collection.building = buildingBoundingBoxes;
   collection.ground = groundBoundingBoxes;
   // collection.<myData> = <myData>Set;

   return collection;
}
#pragma endregion



#pragma region Save
void saveAABB(const std::string& filePath, const std::map<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>>& AABBs)
{
   std::filebuf fb;
   fb.open(filePath, std::ios::out);

   std::ostream file(&fb);

   file << AABBs.size() << "\n";

   for (std::pair<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>> p : AABBs)
   {
      file << p.first << "\n";
      file << std::fixed << p.second.first.x << "\n"; //std::fixed -> important pour conserver tous les chiffres significatifs (ne pas avoir de 1e19)
      file << std::fixed << p.second.first.y << "\n";
      file << std::fixed << p.second.first.z << "\n";
      file << std::fixed << p.second.second.x << "\n";
      file << std::fixed << p.second.second.y << "\n";
      file << std::fixed << p.second.second.z << "\n";
   }

   fb.close();
}
#pragma endregion



#pragma region Build AABB
std::map<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>> buildAABB(const std::string& cityGmlDirectory, const TiledLayer& tile, const CityObjectsType& type)
{
   std::map<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>> AABBs;

   for (int x = tile.TuileMinX; x <= tile.TuileMaxX; ++x)
   {
      for (int y = tile.TuileMinY; y <= tile.TuileMaxY; ++y)
      {
         std::string FileName = cityGmlDirectory + tile.Name + "/" + std::to_string(x) + "_" + std::to_string(y) + "/" + std::to_string(x) + "_" + std::to_string(y) + tile.Name + ".gml";

         glm::highp_dvec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
         glm::highp_dvec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

         fs::path File(FileName);
         if (fs::exists(File))
         {
            std::vector<std::shared_ptr<Triangle>>* triangles = BuildTriangleList(FileName, type);

            for (const auto& t: (*triangles)) //Pour eliminer les points anormaux (qui ont des coordonnees z absurdes), on fait un petit filtre en verifiant ces valeurs de z.
            {
               min.x = std::min(t->a.x, min.x); min.y = std::min(t->a.y, min.y); if (t->a.z > -500) min.z = std::min(t->a.z, min.z);
               min.x = std::min(t->b.x, min.x); min.y = std::min(t->b.y, min.y); if (t->b.z > -500) min.z = std::min(t->b.z, min.z);
               min.x = std::min(t->c.x, min.x); min.y = std::min(t->c.y, min.y); if (t->b.z > -500) min.z = std::min(t->c.z, min.z);
               max.x = std::max(t->a.x, max.x); max.y = std::max(t->a.y, max.y); if (t->a.z < 1000) max.z = std::max(t->a.z, max.z);
               max.x = std::max(t->b.x, max.x); max.y = std::max(t->b.y, max.y); if (t->b.z < 1000) max.z = std::max(t->b.z, max.z);
               max.x = std::max(t->c.x, max.x); max.y = std::max(t->c.y, max.y); if (t->c.z < 1000) max.z = std::max(t->c.z, max.z);
            }
            delete triangles;
         }

         AABBs.insert(std::make_pair(tile.Name + "/" + std::to_string(x) + "_" + std::to_string(y) + "/" + std::to_string(x) + "_" + std::to_string(y) + tile.Name + ".gml", std::make_pair(min, max)));
         spdlog::info("File : {}/{}_{}/{}_{}{}.gml", tile.Name, std::to_string(x), std::to_string(y), std::to_string(x), std::to_string(y), tile.Name);
      }
   }

   return AABBs;
}



void buildLayersAABBs(const std::string& cityGmlDirectory)
{
   TiledFiles Files(cityGmlDirectory);
   Files.BuildListofLayers();

   for (TiledLayer L : Files.ListofLayers)
   {
      CityObjectsType type;
      if (L.Name.find("_BATI") != std::string::npos)
         type = CityObjectsType::COT_Building;
      else if (L.Name.find("_MNT") != std::string::npos)
         type = CityObjectsType::COT_TINRelief;
      else if (L.Name.find("_WATER") != std::string::npos)
         type = CityObjectsType::COT_WaterBody;
      else if (L.Name.find("_VEGET") != std::string::npos)
      {
         type = CityObjectsType::COT_SolitaryVegetationObject;
      }
      else
      {
         // Dummy value acting as a default fuse value:
         type = CityObjectsType::COT_All;
      }

      // Pour chaque tuile "string", bounding box : min-max
      std::map<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>> AABBs =
         buildAABB(cityGmlDirectory, L, type);

      saveAABB(cityGmlDirectory + L.Name + "_AABB.dat", AABBs);
   }

   spdlog::info("Done.");
}



void buildBuildingAABBs(const std::string& filepath)
{
   Tile* tile = new Tile(filepath);

   CityModel* city = tile->getCityModel();

   //Size of Building Parts AABB
   size_t AABBsize = 0;

   //Create and open files
   std::ofstream ofs_B_AABB; // Building AABB
   std::ofstream ofs_BP_AABB; // Building Parts AABB

   size_t extensionPos = filepath.find(".gml");
   std::string filename_B_AABB = filepath.substr(0, extensionPos) + "_Building_AABB.dat";
   std::string filename_BP_AABB = filepath.substr(0, extensionPos) + "_BuildingParts_AABB.dat";

   ofs_B_AABB.open(filename_B_AABB, std::ofstream::trunc);

   CityObjects cityobjects = city->getCityObjectsRoots();

   //Write in Building AABB file
   ofs_B_AABB << cityobjects.size() << std::endl;

   for (const CityObject* cityObj : cityobjects)
   {
      Envelope envCityObj = cityObj->getEnvelope();

      //Print AABB in Building AABB file
      ofs_B_AABB << cityObj->getId() << std::endl;
      ofs_B_AABB << std::to_string(envCityObj.getLowerBound().x) << std::endl;
      ofs_B_AABB << std::to_string(envCityObj.getLowerBound().y) << std::endl;
      ofs_B_AABB << std::to_string(envCityObj.getLowerBound().z) << std::endl;
      ofs_B_AABB << std::to_string(envCityObj.getUpperBound().x) << std::endl;
      ofs_B_AABB << std::to_string(envCityObj.getUpperBound().y) << std::endl;
      ofs_B_AABB << std::to_string(envCityObj.getUpperBound().z) << std::endl;

      std::vector<CityObject*> cityobjchilds = cityObj->getChildren();

      AABBsize += cityobjchilds.size();
   }

   ofs_B_AABB.close();

   //Write in Building Parts AABB File
   ofs_BP_AABB.open(filename_BP_AABB, std::ofstream::trunc);

   ofs_BP_AABB << AABBsize << std::endl;

   for (const CityObject* cityObj : cityobjects)
   {
      std::vector<CityObject*> cityobjchilds = cityObj->getChildren();

      for (CityObject* subObj : cityobjchilds)
      {
         Envelope envSubObj = subObj->getEnvelope();

         //Print AABB in Building Parts AABB file
         ofs_BP_AABB << subObj->getId() << std::endl;
         ofs_BP_AABB << std::to_string(envSubObj.getLowerBound().x) << std::endl;
         ofs_BP_AABB << std::to_string(envSubObj.getLowerBound().y) << std::endl;
         ofs_BP_AABB << std::to_string(envSubObj.getLowerBound().z) << std::endl;
         ofs_BP_AABB << std::to_string(envSubObj.getUpperBound().x) << std::endl;
         ofs_BP_AABB << std::to_string(envSubObj.getUpperBound().y) << std::endl;
         ofs_BP_AABB << std::to_string(envSubObj.getUpperBound().z) << std::endl;

      }
   }

   ofs_BP_AABB.close();

   delete tile;
}



void buildBuildingAABBsInDirectory(const std::string& buildingFilesFolder)
{
   fs::path parentPath(buildingFilesFolder);

   //Loop recursively through folder
   for (const auto& it : fs::recursive_directory_iterator(parentPath))
   {
      if (fs::is_directory(it))
         continue;

      std::string filePath(it.path().string());
      if (!filePath.find("_BATI"))
         continue;

      std::string fileName(it.path().filename().string());
      if (!fileName.ends_with(".gml"))
         continue;

      spdlog::info("File : {}", fileName);
      buildBuildingAABBs(filePath);
   }
}
#pragma endregion

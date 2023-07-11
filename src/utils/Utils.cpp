#include <algorithm>
#include <vector>

#include "Utils.h"

std::ostream& operator<<(std::ostream& os, const glm::highp_dvec3& v)
{
    return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
    return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
    return os << '(' << v.x << ", " << v.y << ')';
}

std::istream& operator>>(std::istream& is, glm::highp_dvec3 & v)
{
    return is >> v.x >> v.y >> v.z;
}

std::istream& operator>>(std::istream& is, glm::vec3& v)
{
    return is >> v.x >> v.y >> v.z;
}

std::istream& operator>>(std::istream& is, glm::vec2& v)
{
    return is >> v.x >> v.y;
}

std::vector<FileInfo> searchGmlFilesInLayer(const fs::directory_entry& layerFolder)
{
   std::vector<FileInfo> result;

   // Loop in tile Folder
   for (const auto& tileFolder : fs::directory_iterator(layerFolder))
   {
      if (!fs::is_directory(tileFolder))
         continue;

      // Loop in tiles
      for (const auto& tile : fs::directory_iterator(tileFolder))
      {
         if (fs::is_directory(tile))
            continue;

         if (tile.path().filename().string().ends_with(".gml"))
         {
            auto tileAbsolutePath (fs::absolute(tile.path()));
            result.push_back(FileInfo(tileAbsolutePath.string()));
         }
      }
   }

   return result;
}

std::vector<FileInfo> getAllTilesIn(const std::string& cityGmlDirectory)
{
   std::vector<FileInfo> result;

   fs::path parentPath(cityGmlDirectory);

   // Loop in layer folder
   for (const auto& layerFolder : fs::directory_iterator(parentPath))
   {
      if (!fs::is_directory(layerFolder))
         continue;

      const std::string layerFileName (layerFolder.path().filename().string());

      // Is a layer folder
      if (!(layerFileName == "_BATI" || layerFileName == "_MNT"))
         continue;

      auto temp (searchGmlFilesInLayer(layerFolder));
      result.insert(result.end(), temp.begin(), temp.end());
   }

   return result;
}

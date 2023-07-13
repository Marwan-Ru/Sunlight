// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "FileInfo.h"

#include <iostream>

namespace fs = std::filesystem;

FileInfo::FileInfo(std::string filepath)
{
   m_filePath = fs::path(filepath);

   //Find type from path
   if (m_filePath.string().find("_BATI") != std::string::npos)
      m_type = CityGMLFileType::_BATI;
   else if (m_filePath.string().find("_MNT") != std::string::npos)
      m_type = CityGMLFileType::_MNT;
}

const std::string FileInfo::getPath() const
{
   return m_filePath.string();
}

const CityGMLFileType& FileInfo::getType() const
{
   return m_type;
}

std::string FileInfo::withGMLExtension() const
{
   return m_filePath.filename().string();
}

std::string FileInfo::withPrevFolder() const
{
   fs::path folderPath (m_filePath);
   folderPath.replace_extension("");
   
   std::string filename (folderPath.filename().string());

    if (m_type == CityGMLFileType::_BATI)
        return "_BATI/" + filename;
    else if (m_type == CityGMLFileType::_MNT)
        return "_MNT/" + filename;
    else
        return "";
}

std::string FileInfo::withPrevFolderAndGMLExtension() const
{
    return withPrevFolder() + ".gml";
}

std::string FileInfo::getPathForBoundingBox() const
{
   fs::path boundingBoxPath (m_filePath);
   boundingBoxPath.replace_extension("");

   return boundingBoxPath.string() + "_Building_AABB.dat";
}

// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "FileInfo.h"

FileInfo::FileInfo(std::string filepath)
{
    m_filepath = filepath;

    //Find filename
    size_t pos_lastfolder = filepath.find_last_of("\\");
    size_t pos_extension = filepath.find(".gml");

    m_filename = filepath.substr(pos_lastfolder + 1, pos_extension - pos_lastfolder - 1);

    //Find type from path
    if (m_filepath.find("_BATI") != std::string::npos)
        m_type = CityGMLFileType::_BATI;
    else if (m_filepath.find("_MNT") != std::string::npos)
        m_type = CityGMLFileType::_MNT;

}

const std::string& FileInfo::getPath() const
{
   return m_filepath;
}

const CityGMLFileType& FileInfo::getType() const
{
   return m_type;
}

std::string FileInfo::withGMLExtension() const
{
    return m_filename + ".gml";
}

std::string FileInfo::withPrevFolder() const
{
    if (m_type == CityGMLFileType::_BATI)
        return "_BATI/" + m_filename;
    else if (m_type == CityGMLFileType::_MNT)
        return "_MNT/" + m_filename;
    else
        return "";
}

std::string FileInfo::withPrevFolderAndGMLExtension() const
{
    return withPrevFolder() + ".gml";
}

std::string FileInfo::getPathForBoundingBox() const
{
   size_t extensionPos = m_filepath.find(".gml");
   std::string boundingBoxPath = m_filepath.substr(0, extensionPos) + "_Building_AABB.dat";

   return boundingBoxPath;
}

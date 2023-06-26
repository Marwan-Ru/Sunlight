// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <string>

enum CityGMLFileType
{
   _DEFAULT,
   _BATI,
   _MNT
};

/**
 * @brief The FileInfo class is an utilitary to manipulate files in the plugin
*/
class FileInfo
{

public:
    FileInfo(std::string filepath);

    /**
     * @brief Get full file path
     * @return 
    */
    const std::string& getPath() const;

    /**
     * @brief Get the file type (_BATI, _MNT...)
     * @return 
    */
    const CityGMLFileType& getType() const;

    /**
     * @brief Get the name of the file with .gml extension
     * @return
    */
    std::string withGMLExtension() const;

    /**
     * @brief Get the name of the file with previous folder. (Ex : '_BATI/3670_10383')
     * @return
    */
    std::string withPrevFolder() const;

    /**
     * @brief Get the name of the file with previous folder and .gml extension (Ex : '_BATI/3670_10383.gml')
     * @return
    */
    std::string withPrevFolderAndGMLExtension() const;

    /**
     * @brief Get path of the current bounding box
     * @return 
    */
    std::string getPathForBoundingBox() const;

private:
    /**
     * @brief File name of the file
    */
    std::string m_filename;

    /**
     * @brief Full path to the path
    */
    std::string m_filepath;

    /**
     * @brief Type of the file (_BATI or _MNT)
    */
    CityGMLFileType m_type;
};

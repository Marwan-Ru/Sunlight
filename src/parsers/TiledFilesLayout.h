// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <string>
#include <vector>

#include <maths/Vectors.h>

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
#pragma warning(disable: 4251) // export problem on STL members
#endif

struct TiledLayer
{
    std::string Name;
    int TuileMinX;
    int TuileMinY;
    int TuileMaxX;
    int TuileMaxY;
};

class TiledFiles
{
public:
    TiledFiles(std::string Folderpath);
    void BuildListofLayers();

    /// Main folder containing all tiled files
    std::string Folder;

    /// Contains all layers present in Folder
    std::vector<TiledLayer> ListofLayers;
};

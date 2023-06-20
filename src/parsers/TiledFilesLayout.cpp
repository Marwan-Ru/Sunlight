// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "TiledFilesLayout.h"

#include <filesystem>
#include <iostream>
#include <map>

#include "citygmls/Tile.h"

namespace fs = std::filesystem;

TiledFiles::TiledFiles(std::string Folderpath)
{
    Folder = Folderpath;
}

void TiledFiles::BuildListofLayers()
{
    const fs::path QFolder(Folder);

    if (!fs::exists(QFolder))
    {
        std::cout << "Error, Folder does not exists." << std::endl;
        return;
    }

    for (auto& LayerFolder : fs::directory_iterator(QFolder))
    {
        if (!LayerFolder.is_directory())
            continue;

        std::string layerFilePath(LayerFolder.path().string());
        std::string layerFileName(LayerFolder.path().filename().string());

        if (layerFilePath.ends_with("ShpExtruded") || layerFilePath.ends_with("SkylineOutput")) //For visibility plugin
            continue;

        if (layerFilePath.ends_with("tmp")) //For FloodAR plugin
            continue;

        fs::path QDirLayerFolder(layerFilePath);

        TiledLayer L;
        // TODO test if it's the correct base name
        L.Name = layerFileName;
        L.TuileMinX = -1;
        L.TuileMinY = -1;
        L.TuileMaxX = -1;
        L.TuileMaxY = -1;

        for (const auto& TileFolder : std::filesystem::directory_iterator(QDirLayerFolder))
        {
            if (!TileFolder.is_directory())
                continue;

            // TODO test if it's the correct base name (directory parent of current file)
            std::string Tile = TileFolder.path().filename().string();

            size_t SplitPos = Tile.find("_"); //Position of the split character "_" between X and Y coordinates of the current tile

            std::string X = Tile.substr(0, SplitPos);
            std::string Y = Tile.substr(SplitPos + 1);

            int Xval = std::stoi(X);
            int Yval = std::stoi(Y);

            if (L.TuileMinX == -1 || L.TuileMinX > Xval)
                L.TuileMinX = Xval;
            if (L.TuileMaxX == -1 || L.TuileMaxX < Xval)
                L.TuileMaxX = Xval;
            if (L.TuileMinY == -1 || L.TuileMinY > Yval)
                L.TuileMinY = Yval;
            if (L.TuileMaxY == -1 || L.TuileMaxY < Yval)
                L.TuileMaxY = Yval;
        }

        std::cout << "Layer : " << L.Name << " : " << L.TuileMinX << " " << L.TuileMinY << std::endl << L.TuileMaxX << " " << L.TuileMaxY << std::endl;

        ListofLayers.push_back(L);
    }
}

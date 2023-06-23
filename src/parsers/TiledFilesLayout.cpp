// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <filesystem>
#include <iostream>
#include <map>
// Log in console
#include <spdlog/spdlog.h>
#include <optional>
#include <glm/vec2.hpp>
// GLM min and max functions
#include <glm/common.hpp>

#include "TiledFilesLayout.h"
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
        spdlog::error("Folder does not exists.");
        return;
    }

    for (auto& LayerFolder : fs::directory_iterator(QFolder))
    {
        if (!LayerFolder.is_directory())
            continue;

        std::optional<glm::ivec2> min;
        std::optional<glm::ivec2> max;

        for (const auto& TileFolder : std::filesystem::directory_iterator(LayerFolder.path()))
        {
            if (!TileFolder.is_directory())
                continue;

            // TODO test if it's the correct base name (directory parent of current file)
            std::string Tile = TileFolder.path().filename().string();

            size_t SplitPos = Tile.find("_"); //Position of the split character "_" between X and Y coordinates of the current tile

            std::string X = Tile.substr(0, SplitPos);
            std::string Y = Tile.substr(SplitPos + 1);

            int Xval (std::stoi(X));
            int Yval (std::stoi(Y));
            glm::ivec2 currentTileCoordinate(Xval, Yval);

            if (!min.has_value())
            {
               min = currentTileCoordinate;
            }
            if (!max.has_value())
            {
               max = currentTileCoordinate;
            }

            // Minimum and maximum of Tile Coordinate
            min = glm::min(min.value(), currentTileCoordinate);
            max = glm::max(max.value(), currentTileCoordinate);
        }

        std::string layerFileName(LayerFolder.path().filename().string());
        if (!min.has_value() && !max.has_value())
        {
           spdlog::warn("Tile {} at {} do not follow the correct folder hierarchy.", layerFileName, QFolder.string());
           continue;
        }

        TiledLayer L;
        // TODO test if it's the correct base name
        L.Name = layerFileName;
        L.TuileMinX = min.value().x;
        L.TuileMinY = min.value().y;
        L.TuileMaxX = max.value().x;
        L.TuileMaxY = max.value().y;


        spdlog::info("Layer : {} - Min X : {} - Min Y : {} - Max X : {} - Max Y : {}", L.Name, L.TuileMinX, L.TuileMinY, L.TuileMaxX, L.TuileMaxY);
        ListofLayers.push_back(L);
    }
}

// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <filesystem>
#include <map>
// Log in console
#include <spdlog/spdlog.h>
#include <optional>
#include <maths/Vector2.h>

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

        // Avoid to loop in non layer folder
        const std::string layerFolder (LayerFolder.path().filename().string());
        if (layerFolder != "_BATI" && layerFolder != "_MNT")
           continue;

        std::optional<TVec2i> min;
        std::optional<TVec2i> max;

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
            TVec2i currentTileCoordinate(Xval, Yval);

            if (!min.has_value())
            {
               min = currentTileCoordinate;
            }
            if (!max.has_value())
            {
               max = currentTileCoordinate;
            }

            min = getMinimumCoordinate(min.value(), currentTileCoordinate);
            max = getMaximumCoordinate(min.value(), currentTileCoordinate);
        }

        const std::string layerFileName(LayerFolder.path().filename().string());
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

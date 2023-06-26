// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <fstream>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>

#include "SunlightCsvExporter.h"
#include "FileInfo.h"
#include <maths/Triangle.h>
#include <utils/DateTime.h>

void createOutputFolders(const std::string& sOutputDir)
{
    //*** Create output folders
    std::filesystem::path outputDirSunlight(sOutputDir + "Sunlight/");
    if (!std::filesystem::exists(outputDirSunlight))
        std::filesystem::create_directory(outputDirSunlight);

    std::filesystem::path outputDirBati(sOutputDir + "Sunlight/_BATI");
    if (!std::filesystem::exists(outputDirBati))
        std::filesystem::create_directory(outputDirBati);

    std::filesystem::path outputDirMnt(sOutputDir + "Sunlight/_MNT");
    if (!std::filesystem::exists(outputDirMnt))
        std::filesystem::create_directory(outputDirMnt);
}

void createFileFolder(FileInfo* file, const std::string& sOutputDir)
{
    //Create folder corresponding to file
    std::filesystem::path path(sOutputDir + "Sunlight/" + file->withPrevFolder() + "/");

    // Replace all existant datas to the lastet computation
    if (std::filesystem::exists(path))
        std::filesystem::remove_all(path);

    std::filesystem::create_directory(path);
}

void exportLightningToCSV(std::map<int, bool>& sunInfo, Triangle* t, FileInfo* file, int iStartDate, int iEndDate, std::string& outputDir)
{
    int nb_days = (iEndDate - iStartDate + 1) / 24;

    for (int day_nb = 0; day_nb < nb_days; ++day_nb)
    {
        //Get day as string
        int iDate = iStartDate + (day_nb * 24);
        std::string datetime = decodeDateTime(iDate);
        std::string day = datetime.substr(0, datetime.find(":"));

        //Create and open file
        std::ofstream ofs;
        ofs.open(outputDir + "/Sunlight/" + file->withPrevFolder() + "/" + day + ".csv", std::ofstream::app);

        for (int i = 0; i < 24; ++i) //For each hour
        {
            ofs << decodeDateTime(iDate + i) << ";" << t->polygonId << ";" << std::to_string(sunInfo[iDate + i]) << std::endl;
        }

        ofs.close();
    }
}

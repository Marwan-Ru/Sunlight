// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <fstream>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
// Log in console
#include <spdlog/spdlog.h>

#include "IO.h"
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
    std::filesystem::path path(sOutputDir + "Sunlight/" + file->WithPrevFolder() + "/");

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
        ofs.open(outputDir + "/Sunlight/" + file->WithPrevFolder() + "/" + day + ".csv", std::ofstream::app);

        for (int i = 0; i < 24; ++i) //For each hour
        {
            ofs << decodeDateTime(iDate + i) << ";" << t->polygonId << ";" << std::to_string(sunInfo[iDate + i]) << std::endl;
        }

        ofs.close();
    }
}

glm::highp_dvec3 rotateVector(const glm::highp_dvec3& vec, const glm::dquat& q)
{
    glm::dquat qConj = glm::conjugate(q);
    glm::dquat qTemp = qConj * glm::dquat(0.0f, vec.x, vec.y, vec.z) * q;
    return glm::highp_dvec3(qTemp.x, qTemp.y, qTemp.z);
}

glm::dquat getSunRotation(double azimutAngleInRadians, double elevationAngleInRadians)
{
    glm::dquat elevationQuaternion (glm::highp_dvec3(-elevationAngleInRadians, 0, 0));
    glm::dquat azimutQuaternion (glm::highp_dvec3(0, 0, azimutAngleInRadians));

    return elevationQuaternion * azimutQuaternion;
}

glm::highp_dvec3 computeDirectionTowardsTheSun(double azimutAngleInRadians, double elevationAngleInRadians)
{
    //if sun to low (angle < 1 degree), return nul beam direction
    if (elevationAngleInRadians <= 0.01 || azimutAngleInRadians <= 0.01)
        return glm::highp_dvec3(0.0, 0.0, 0.0);
   
    //Lambert 93 Coordinates
    glm::highp_dvec3 originOffset (1843927.29, 5173886.65, 0.0);
    //SunPos is the first position of the sun (north) from which the angles are expressed
    glm::highp_dvec3 sunBasePosition (glm::highp_dvec3(0.0, 60000.0, 0.0));
    
    // Rotate the sun base position with the rotation at a given time (or azimut / elevation)
    glm::dquat finalRotation (getSunRotation(azimutAngleInRadians, elevationAngleInRadians));
    glm::highp_dvec3 sunPositionAfterRotation (rotateVector(sunBasePosition, finalRotation) + originOffset);

    //Compute sun beam direction
    glm::highp_dvec3 directionToSun (sunPositionAfterRotation - originOffset);

    return glm::normalize(directionToSun);
}

std::map<int, glm::highp_dvec3> loadSunpathFile(std::string sunPathFile, int iStartDate, int iEndDate)
{
    std::ifstream sunFile(sunPathFile);
    if (!sunFile.is_open())
    {
       spdlog::error("Failed to open {}", sunPathFile);
       return {};
    }

    std::map<int, glm::highp_dvec3> SunBeamDir;

    bool found = false; //Date found
    bool exit_loop = false;

    std::string line;

    //Skip header line
    std::getline(sunFile, line);

    while (std::getline(sunFile, line) && exit_loop == false) // For all lines of csv file
    {
        std::stringstream  lineStream(line);
        std::string        cell;

        std::getline(lineStream, cell, ';'); //Get first cell of row

        std::string sCurrentDate = cell;
        int iCurrentDate = encodeDateTime(cell, 0);

        if (iCurrentDate >= iStartDate && iCurrentDate < iEndDate)
        {
            found = true;
            std::getline(lineStream, cell, ';'); //jump second cell of line

            //*** The following lines needs to be commented out if input sun position values
            //*** already consider time changes of the 27 March and the 30 November

            //Encode 27 march and 30 november (dates when time changes : summer hour <-> winter hour)
            int twentySevenMarch = encodeDateTime(2016, 3, 27, 0);
            int thirtyNovember = encodeDateTime(2016, 11, 30, 0);
            bool timeshift = false;

            if (iCurrentDate < twentySevenMarch || iCurrentDate >= thirtyNovember)
            {
                //Skip first two cells -> shift time back from hour
                std::getline(lineStream, cell, ';');
                std::getline(lineStream, cell, ';');
                timeshift = true;
            }

            //*** End of the part which needs to be commented out

            int hour = 0;
            while (std::getline(lineStream, cell, ';'))
            {
                // 0.0 default value if the sun is low
                double azimutAngleInRadians (0.0);
                if (cell != "--" && cell != "")
                {
                   azimutAngleInRadians = glm::radians(std::stod(cell));
                }

                //Get next cell (elevation angle)
                std::getline(lineStream, cell, ';');

                double elevationAngleInRadians (0.0);
                if (cell != "--" && cell != "")
                {
                   elevationAngleInRadians = glm::radians(std::stod(cell));
                }

                //Encode datetime
                int dateTime = encodeDateTime(sCurrentDate, hour);

                //Compute Sun's beam Direction
                SunBeamDir[dateTime] = computeDirectionTowardsTheSun(azimutAngleInRadians, elevationAngleInRadians);

                ++hour;
            }


            //*** The following lines needs to be commented out if input sun position values
            //*** already consider time changes of the 27 March and the 30 November

            if (timeshift)
            {
                //Add nul beam direction for last hour of the day
                int dateTime = encodeDateTime(sCurrentDate, hour);
                SunBeamDir[dateTime] = glm::highp_dvec3(0.0, 0.0, 0.0);
            }

            //*** End of the part which needs to be commented out

        }
        else if (iCurrentDate > iEndDate)
            exit_loop = true;
    }

    if (found == false)
    {
       spdlog::error("Do not found any date corresponding while loading Annual SunPath file.");
    }

    return SunBeamDir;
}

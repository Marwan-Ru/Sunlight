// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <fstream>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>

#include "IO.h"
#include "FileInfo.h"
#include <maths/Triangle.h>
#include <utils/DateTime.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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

glm::highp_dvec3 RotateVector(const glm::highp_dvec3& vec, const glm::dquat& q)
{
    glm::dquat qConj = glm::conjugate(q);
    glm::dquat qTemp = qConj * glm::dquat(0.0f, vec.x, vec.y, vec.z) * q;
    return glm::highp_dvec3(qTemp.x, qTemp.y, qTemp.z);
}

glm::dquat computeSunRotation(double azimutAngleInRadians, double elevationAngleInRadians)
{
    glm::dquat elevationQuaternion(glm::highp_dvec3(-elevationAngleInRadians, 0, 0));
    glm::dquat azimutQuaternion(glm::highp_dvec3(0, 0, azimutAngleInRadians));

    return elevationQuaternion * azimutQuaternion;
}

///
/// \brief computeBeamDir Compute sun direction given an azimut and elevation angle
/// \param azimutAngle Azimut angle of the sun
/// \param elevationAngle Elevation angle of the sun
/// \return Beam normalized direction vector
///
glm::highp_dvec3 computeBeamDir(double azimutAngle, double elevationAngle)
{
    //if sun to low (angle < 1 degree), return nul beam direction
    if (elevationAngle <= 0.01 || azimutAngle <= 0.01)
        return glm::highp_dvec3(0.0, 0.0, 0.0);
   
    //Lambert 93 Coordinates
    glm::highp_dvec3 originOffset (1843927.29, 5173886.65, 0.0);
    //SunPos is the first position of the sun (north) from which the angles are expressed
    glm::highp_dvec3 sunBasePosition (glm::highp_dvec3(0.0, 60000.0, 0.0));
    
    // Rotate the sun base position with the rotation at a given time (or azimut / elevation)
    glm::dquat finalRotation(computeSunRotation(azimutAngle, elevationAngle));
    glm::highp_dvec3 sunPositionAfterRotation (RotateVector(sunBasePosition, finalRotation) + originOffset);

    //Compute sun beam direction
    glm::highp_dvec3 directionToSun (sunPositionAfterRotation - originOffset);

    return glm::normalize(directionToSun);
}

std::map<int, glm::highp_dvec3> loadSunpathFile(std::string sunpathFile, int iStartDate, int iEndDate)
{
    std::map<int, glm::highp_dvec3> SunBeamDir;

    std::ifstream file(sunpathFile); //Load file
    std::string line;

    bool found = false; //Date found
    bool exit_loop = false;

    //Skip header line
    std::getline(file, line);

    while (std::getline(file, line) && exit_loop == false) // For all lines of csv file
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
                //Get azimutAngle
                double azimutAngle;

                if (cell == "--" || cell == "")
                    azimutAngle = 0.0;
                else
                    azimutAngle = std::stod(cell) * M_PI / 180; //Conversion to radian

                //Get next cell (elevation angle)
                std::getline(lineStream, cell, ';');

                double elevationAngle;

                //Add Elevation angle
                if (cell == "--" || cell == "")
                    elevationAngle = 0.0;
                else
                    elevationAngle = std::stod(cell) * M_PI / 180;

                //Encode datetime
                int dateTime = encodeDateTime(sCurrentDate, hour);

                //Compute Sun's beam Direction
                SunBeamDir[dateTime] = computeBeamDir(azimutAngle, elevationAngle);

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
        std::cerr << "Error while loading Annual SunPath file." << std::endl;

    return SunBeamDir;
}

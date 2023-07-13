#include <fstream>
// Log in console
#include <spdlog/spdlog.h>
#include <maths/maths.h>

#include "SunEarthParser.h"
#include <utils/DateTime.h>

SunEarthToolsParser::SunEarthToolsParser(const std::string& sunPathFile, int iStartDate, int iEndDate)
{
   m_directionTowardsTheSun = loadSunpathFile(sunPathFile, iStartDate, iEndDate);
}

std::map<int, TVec3d> SunEarthToolsParser::loadSunpathFile(const std::string& sunPathFile, int iStartDate, int iEndDate)
{
   std::ifstream sunFile(sunPathFile);
   if (!sunFile.is_open())
   {
      spdlog::error("Failed to open {}", sunPathFile);
      return {};
   }

   std::map<int, TVec3d> SunBeamDir;

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
            double azimutAngleInRadians(0.0);
            if (cell != "--" && cell != "")
            {
               azimutAngleInRadians = radians(std::stod(cell));
            }

            //Get next cell (elevation angle)
            std::getline(lineStream, cell, ';');

            double elevationAngleInRadians(0.0);
            if (cell != "--" && cell != "")
            {
               elevationAngleInRadians = radians(std::stod(cell));
            }

            //Encode datetime
            int dateTime = encodeDateTime(sCurrentDate, hour);

            //Compute Sun's beam Direction
            SunBeamDir[dateTime] = computeDirectionTowardsTheSun(azimutAngleInRadians, elevationAngleInRadians);
            if (SunBeamDir[dateTime] == TVec3d(0, 0, 0))
            {
               spdlog::warn("Sun is too low to compute sunlight for {} {} with azimut angle ({}) and elevation angle ({})", sCurrentDate, hour, azimutAngleInRadians, elevationAngleInRadians);
            }

            ++hour;
         }


         //*** The following lines needs to be commented out if input sun position values
         //*** already consider time changes of the 27 March and the 30 November

         if (timeshift)
         {
            //Add nul beam direction for last hour of the day
            int dateTime = encodeDateTime(sCurrentDate, hour);
            SunBeamDir[dateTime] = TVec3d(0.0, 0.0, 0.0);
            spdlog::warn("Timeshift detected, so the position for {} {} will not be computed", sCurrentDate, hour);
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

const std::map<int, TVec3d>& SunEarthToolsParser::getDirectionTowardsTheSun() const
{
   return m_directionTowardsTheSun;
}

Quaternion SunEarthToolsParser::getSunRotation(double azimutAngleInRadians, double elevationAngleInRadians)
{
   Quaternion elevationQuaternion (TVec3d(-1.0, 0, 0), elevationAngleInRadians);
   Quaternion azimutQuaternion(TVec3d(0, 0, 1.0), azimutAngleInRadians);

   return elevationQuaternion * azimutQuaternion;
}

TVec3d SunEarthToolsParser::computeDirectionTowardsTheSun(double azimutAngleInRadians, double elevationAngleInRadians)
{
   //if sun to low (angle < 1 degree), return nul beam direction
   if (elevationAngleInRadians <= 0.01 || azimutAngleInRadians <= 0.01)
      return TVec3d(0.0, 0.0, 0.0);

   //Lambert 93 Coordinates
   TVec3d originOffset(1843927.29, 5173886.65, 0.0);
   //SunPos is the first position of the sun (north) from which the angles are expressed
   TVec3d sunBasePosition(TVec3d(0.0, 60000.0, 0.0));

   // Rotate the sun base position with the rotation at a given time (or azimut / elevation)
   Quaternion finalRotation(getSunRotation(azimutAngleInRadians, elevationAngleInRadians));
   TVec3d sunPositionAfterRotation(finalRotation * sunBasePosition + originOffset);

   //Compute sun beam direction
   TVec3d directionToSun(sunPositionAfterRotation - originOffset);

   return directionToSun.normalize();
}

#include <fstream>
#include <sstream>
// Log in console
#include <spdlog/spdlog.h>

#include "SunEarthParser.h"
#include <utils/DateTime.h>
#include <maths/maths.h>

void SunEarthToolsParser::loadSunpathFile(const std::string& sunPathFile, int iStartDate, int iEndDate)
{
   std::ifstream sunFile(sunPathFile);
   if (!sunFile.is_open())
   {
      spdlog::error("Failed to open {}", sunPathFile);
      return;
   }

   bool found = false; //Date found
   bool exit_loop = false;

   std::string line;

   //Skip header line
   std::getline(sunFile, line);

   while (std::getline(sunFile, line) && exit_loop == false) // For all lines of csv file
   {
      std::stringstream  lineStream (line);
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
            auto sunPosition (computeSunPosition(azimutAngleInRadians, elevationAngleInRadians));
            auto sunDirection (computeSunDirection(sunPosition));

            if (sunPosition != TVec3d(0, 0, 0))
            {
               m_sunDatasLoaded.push_back(SunDatas(decodeDateTime(dateTime), sunPosition, sunDirection));
            }
            else
            {
               spdlog::warn("Sun is too low at {} {} to compute sunlight with azimut angle ({}) and elevation angle ({})", sCurrentDate, hour, azimutAngleInRadians, elevationAngleInRadians);
            }

            ++hour;
         }


         //*** The following lines needs to be commented out if input sun position values
         //*** already consider time changes of the 27 March and the 30 November

         if (timeshift)
         {
            //Add nul beam direction for last hour of the day
            int dateTime = encodeDateTime(sCurrentDate, hour);

            //m_sunDatasLoaded.push_back(SunDatas(decodeDateTime(dateTime), TVec3d(0.0, 0.0, 0.0), TVec3d(0.0, 0.0, 0.0)));
            spdlog::warn("Timeshift detected, so the position for {} {} will not be computed", sCurrentDate, hour);
         }

         //*** End of the part which needs to be commented out

      }
      else if (iCurrentDate > iEndDate)
         exit_loop = true;
   }

   if (found == false)
   {
      spdlog::error("Did not find any date corresponding to request while loading Annual SunPath file.");
   }
}

const std::vector<SunDatas>& SunEarthToolsParser::getSunDatas() const
{
   return m_sunDatasLoaded;
}

Quaternion SunEarthToolsParser::getSunRotation(double azimutAngleInRadians, double elevationAngleInRadians) const
{
   Quaternion elevationQuaternion (TVec3d(-1.0, 0, 0), elevationAngleInRadians);
   Quaternion azimutQuaternion(TVec3d(0, 0, 1.0), azimutAngleInRadians);

   return elevationQuaternion * azimutQuaternion;
}

TVec3d SunEarthToolsParser::computeSunPosition(double azimutAngleInRadians, double elevationAngleInRadians) const
{
   //if sun to low (angle < 1 degree), return nul beam direction
   if (elevationAngleInRadians <= 0.01 || azimutAngleInRadians <= 0.01)
      return TVec3d(0.0, 0.0, 0.0);

   Quaternion finalRotation(getSunRotation(azimutAngleInRadians, elevationAngleInRadians));

   // Rotate the sun base position with the rotation at a given time (or azimut / elevation)
   return finalRotation * SUN_BASE_POSITION + ORIGIN_OFFSET;
}

TVec3d SunEarthToolsParser::computeSunDirection(const TVec3d& sunPosition) const
{
   TVec3d directionToSun(sunPosition - ORIGIN_OFFSET);
   return directionToSun.normalize();
}

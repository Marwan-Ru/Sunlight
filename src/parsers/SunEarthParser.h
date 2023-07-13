#pragma once

#include <string>
#include <map>
#include <vector>
#include <maths/Vector3.h>
#include <maths/Quaternion.h>
#include <maths/Vector3.h>

/**
 * @brief Parser of sun position using this website https://www.sunearthtools.com/dp/tools/pos_sun.php?lang=fr
*/
class SunEarthToolsParser
{
public:
   SunEarthToolsParser(const std::string& sunPathFile, int iStartDate, int iEndDate);

   /**
    * @brief Load a file containing the sun position for a year and filter by a given start and end date
    * @param sunpathFile Absolute path to the file
    * @param iStartDate Start date of sunlight computation encoded as int
    * @param iEndDate End date of sunlight computation encoded as int
    * @return
   */
   std::map<int, TVec3d> loadSunpathFile(const std::string& sunPathFile, int iStartDate, int iEndDate);

   /**
    * @brief Get all direction towards at several timestamp
    * @return 
   */
   const std::map<int, TVec3d>& getDirectionTowardsTheSun() const;

private:
   /**
    * @brief Compute sun rotation given an azimut and elevation angle
    * @param azimutAngleInRadians Azimut angle in radians of the sun
    * @param elevationAngleInRadians Elevation angle in radians of the sun
    * @return
   */
   Quaternion getSunRotation(double azimutAngleInRadians, double elevationAngleInRadians);

   /**
   * @brief Compute sun direction given an azimut and elevation angle in radians
   * @param azimutAngleInRadians Azimut angle in radians of the sun
   * @param elevationAngleInRadians Elevation angle in radians of the sun
   * @return Normalized direction towards the sun
   */
   TVec3d computeDirectionTowardsTheSun(double azimutAngleInRadians, double elevationAngleInRadians);

private:
   /**
    * @brief All directions towards the sun loaded
   */
   std::map<int, TVec3d> m_directionTowardsTheSun;
};

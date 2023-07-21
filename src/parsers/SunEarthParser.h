#pragma once

#include <string>
#include <vector>

#include <maths/Vector3.h>
#include <maths/Quaternion.h>
#include <cores/SunDatas.h>

/**
 * @brief Parser of sun position using this website https://www.sunearthtools.com/dp/tools/pos_sun.php?lang=fr
*/
class SunEarthToolsParser
{
public:
   SunEarthToolsParser() = default;

   /**
    * @brief Load a file containing the sun position for a year and filter by a given start and end date.
    * @param sunpathFile Absolute path to the file.
    * @param iStartDate Start date of sunlight computation encoded as int.
    * @param iEndDate End date of sunlight computation encoded as int.
    * @return
   */
   std::vector<SunDatas> loadSunpathFile(const std::string& sunPathFile, int iStartDate, int iEndDate);

   /**
    * @brief Get all direction towards at several timestamp.
    * @return 
   */
   const std::vector<SunDatas>& getSunDatas() const;

private:
   /**
    * @brief Compute sun rotation given an azimut and elevation angle in radians.
    * @param azimutAngleInRadians Azimut angle in radians of the sun.
    * @param elevationAngleInRadians Elevation angle in radians of the sun.
    * @return
   */
   Quaternion getSunRotation(double azimutAngleInRadians, double elevationAngleInRadians) const;

   /**
   * @brief Compute sun position given an azimut and elevation angle in radians.
   * @param azimutAngleInRadians Azimut angle in radians of the sun.
   * @param elevationAngleInRadians Elevation angle in radians of the sun.
   * @return Sun position at a givent azimut and elevation angle.
   */
   TVec3d computeSunPosition(double azimutAngleInRadians, double elevationAngleInRadians) const;

   /**
   * @brief Compute sun direction with origin offset coordinate.
   * @param sunPosition Sun position in Lambert 93 coordinate.
   * @return 
   */
   TVec3d computeSunDirection(const TVec3d& sunPosition) const;

private:
   /**
    * @brief All sun datas currently loaded.
   */
   std::vector<SunDatas> m_sunDatasLoaded;

   /**
    * @brief Origin offset in Lambert 93 Coordinates
   */
   TVec3d ORIGIN_OFFSET = TVec3d(1843927.29, 5173886.65, 0.0);

   /**
    * @brief First position of the sun (north) from which the angles are expressed
   */
   TVec3d SUN_BASE_POSITION = TVec3d(0.0, 60000.0, 0.0);
};

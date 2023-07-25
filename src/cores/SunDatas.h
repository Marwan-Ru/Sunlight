#pragma once

#include <string>

#include <maths/Vector3.h>

/**
 * @brief Associate sun position to a given date
*/
struct SunDatas
{
   SunDatas() = default;

   SunDatas(const std::string& dateStr, const TVec3d& position, const TVec3d& direction)
      : dateStr(dateStr), position(position), direction(direction)
   {
   }
   
   /**
    * @brief Date formated in string
   */
   std::string dateStr;

   /**
    * @brief Sun position in Lamer 93 coordinate
   */
   TVec3d position;

   TVec3d direction;
};
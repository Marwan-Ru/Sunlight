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
   
   std::string dateStr;
   TVec3d position;
   TVec3d direction;
};
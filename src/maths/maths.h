#pragma once

#include <numbers>

/**
 * @brief Convert degree to radians
 * @param degree 
 * @return 
*/
double radians(double degree)
{
   return degree * std::numbers::pi / 180;
}

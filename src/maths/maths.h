#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

/**
 * @brief Convert degree to radians
 * @param degree 
 * @return 
*/
double radians(double degree)
{
   return degree * M_PI / 180;
}

#include <algorithm>
#include <vector>

#include "Utils.h"

std::ostream& operator<<(std::ostream& os, const TVec3d& v)
{
    return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

std::ostream& operator<<(std::ostream& os, const TVec3f& v)
{
    return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

std::ostream& operator<<(std::ostream& os, const TVec2f& v)
{
    return os << '(' << v.x << ", " << v.y << ')';
}

std::istream& operator>>(std::istream& is, TVec3d & v)
{
    return is >> v.x >> v.y >> v.z;
}

std::istream& operator>>(std::istream& is, TVec3f& v)
{
    return is >> v.x >> v.y >> v.z;
}

std::istream& operator>>(std::istream& is, TVec2f& v)
{
    return is >> v.x >> v.y;
}

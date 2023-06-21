#include "Utils.h" 

std::ostream& operator<<(std::ostream& os, const glm::highp_dvec3& v)
{
    return os << '(' << v.x << ', ' << v.y << ', ' << v.z << ')';
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
    return os << '(' << v.x << ', ' << v.y << ', ' << v.z << ')';
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v)
{
    return os << '(' << v.x << ', ' << v.y << ')';
}

std::istream& operator>>(std::istream& is, glm::highp_dvec3 & v)
{
    return is >> v.x >> v.y >> v.z;
}

std::istream& operator>>(std::istream& is, glm::vec3& v)
{
    return is >> v.x >> v.y >> v.z;
}

std::istream& operator>>(std::istream& is, glm::vec2& v)
{
    return is >> v.x >> v.y;
}
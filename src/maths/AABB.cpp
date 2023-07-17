// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "AABB.h"

AABB::AABB(const TVec3d& _min, const TVec3d& _max, const std::string& id, const std::string& tileName) :
      min(_min), max(_max), m_id(id), m_tileName(tileName)
{
}

//AABB
bool AABB::operator==(AABB const& other)
{
    return name == other.name;
}

const std::string& AABB::getId() const
{
   return m_id;
}

const std::string& AABB::getTileName() const
{
   return m_tileName;
}


//BoxOrder

bool operator<(const BoxOrder& a, const BoxOrder& b)
{
    return a.order < b.order;
}

bool operator<(const BoxwithRays& a, const BoxwithRays& b)
{
    return a.minDistance < b.minDistance;
}

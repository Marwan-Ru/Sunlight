// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "AABB.h"

//AABB
bool AABB::operator==(AABB const& other)
{
    return name == other.name;
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

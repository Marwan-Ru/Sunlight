// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/intersect.hpp> 

#include "Ray.h"
#include "Hit.h"
#include "Triangle.h"

//Ray

Ray::Ray(glm::highp_dvec3 ori, glm::highp_dvec3 dir, int id)
{
    this->id = id;
    this->origin = ori;
    this->direction = dir;
    inv_direction = glm::highp_dvec3(1 / dir.x, 1 / dir.y, 1 / dir.z);
    sign[0] = (inv_direction.x < 0);
    sign[1] = (inv_direction.y < 0);
    sign[2] = (inv_direction.z < 0);
    fragCoord.x = -1;
    fragCoord.y = -1;
}

//Ray triangle intersection, from geometric tools engine
//License : http://www.boost.org/LICENSE_1_0.txt
bool Ray::Intersect(Triangle* triangle, Hit* hit)
{
    Hit tempHit;
    if (glm::intersectLineTriangle(origin, direction, triangle->a, triangle->b, triangle->c, tempHit.impactPoint))
    {
        tempHit.triangle = *triangle;
        tempHit.ray = (*this);

        if (hit != nullptr)
        {
            *hit = tempHit;
        }

        return true;
    }

    return false;
}

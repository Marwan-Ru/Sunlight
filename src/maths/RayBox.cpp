// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
# pragma warning(disable:4251) // gdal-1.11.4 internals (cpl_string.h) when
#endif                         // including ogrsf_frmts.h on VCC++

#include "RayBox.h"
#include "RayTracing.h"

//RayBoxHit

bool operator<(const RayBoxHit& a, const RayBoxHit& b)
{
    return a.minDistance < b.minDistance;
}

//RayBox

RayBox::RayBox(glm::highp_dvec3 ori, glm::highp_dvec3 dir, int id)
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
    boxes = std::vector<RayBoxHit>();
}


//Ray aabb intersection, from pbrt-v2
//License : http://www.pbrt.org/LICENSE.txt
bool RayBox::Intersect(AABB box, float* hitt0, float* hitt1)
{
    double t0 = 0, t1 = FLT_MAX;
    for (int i = 0; i < 3; ++i) {
        // Update interval for _i_th bounding box slab
        double invRayDir = 1. / direction[i];
        double tNear = (box.min[i] - origin[i]) * invRayDir;
        double tFar = (box.max[i] - origin[i]) * invRayDir;

        // Update parametric interval from slab intersection $t$s
        if (tNear > tFar) std::swap(tNear, tFar);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1) return false;
    }
    if (hitt0) *hitt0 = static_cast<float>(t0);
    if (hitt1) *hitt1 = static_cast<float>(t1);
    return true;
}

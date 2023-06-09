// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
# pragma warning(disable:4251) // gdal-1.11.4 internals (cpl_string.h) when
#endif                         // including ogrsf_frmts.h on VCC++

#include "RayBox.h"

//RayBoxHit

bool operator<(const RayBoxHit& a, const RayBoxHit& b)
{
    return a.minDistance < b.minDistance;
}

//RayBoxCollection

RayBoxCollection::RayBoxCollection(std::vector<RayBox*> raysBoxes)
{
    this->raysBB = raysBoxes;
}

RayBoxCollection::~RayBoxCollection()
{
    for (unsigned int i = 0; i < raysBB.size(); ++i)
        delete raysBB[i];
}

//RayBox

RayBox::RayBox(TVec3d ori, TVec3d dir, int id)
{
    this->id = id;
    this->ori = ori;
    this->dir = dir;
    inv_dir = TVec3d(1 / dir.x, 1 / dir.y, 1 / dir.z);
    sign[0] = (inv_dir.x < 0);
    sign[1] = (inv_dir.y < 0);
    sign[2] = (inv_dir.z < 0);
    fragCoord.x = -1;
    fragCoord.y = -1;
    boxes = std::vector<RayBoxHit>();
}


//Ray aabb intersection, from pbrt-v2
//License : http://www.pbrt.org/LICENSE.txt
bool RayBox::Intersect(AABB box, float* hitt0, float* hitt1)
{
    float t0 = 0, t1 = FLT_MAX;
    for (int i = 0; i < 3; ++i) {
        // Update interval for _i_th bounding box slab
        float invRayDir = 1.f / dir[i];
        float tNear = (box.min[i] - ori[i]) * invRayDir;
        float tFar = (box.max[i] - ori[i]) * invRayDir;

        // Update parametric interval from slab intersection $t$s
        if (tNear > tFar) std::swap(tNear, tFar);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar < t1 ? tFar : t1;
        if (t0 > t1) return false;
    }
    if (hitt0) *hitt0 = t0;
    if (hitt1) *hitt1 = t1;
    return true;
}

// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "Quaternion.h"

#include <cmath>

Quaternion::Quaternion()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 0.0;
}

Quaternion::Quaternion(double x, double y, double z, double w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion::Quaternion(const TVec3d& axis, const double angle)
{
    double sinTheta = std::sin(angle / 2.0);
    double cosTheta = std::cos(angle / 2.0);
    TVec3d normalAxis = axis.normalize();

    this->x = normalAxis.x * sinTheta;
    this->y = normalAxis.y * sinTheta;
    this->z = normalAxis.z * sinTheta;
    this->w = cosTheta;
}

Quaternion operator*(const Quaternion& q, double s)
{
    Quaternion qOut = q;

    qOut.x = qOut.x * s;
    qOut.y = qOut.y * s;
    qOut.z = qOut.z * s;
    qOut.w = qOut.w * s;

    return qOut;
}

Quaternion operator*(double s, const Quaternion& q)
{
    return q * s;
}

TVec3d operator*(Quaternion const& q, TVec3d const& vec)
{
    //Compute conjugated Quaternion
    Quaternion qConj = Quaternion(-q.x, -q.y, -q.z, q.w);

    Quaternion qTemp = qConj * Quaternion(vec.x, vec.y, vec.z, 0.0) * q;

    return TVec3d(qTemp.x, qTemp.y, qTemp.z);
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
    return Quaternion(q1.x * q2.w + q1.w * q2.x + q1.y * q2.z - q1.z * q2.y,
        q1.y * q2.w + q1.w * q2.y + q1.z * q2.x - q1.x * q2.z,
        q1.z * q2.w + q1.w * q2.z + q1.x * q2.y - q1.y * q2.x,
        q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}

std::ostream& operator<<(std::ostream& stream, const Quaternion& q)
{
    stream << q.x << " ; " << q.y << " ; " << q.z << " ; " << q.w;
    return stream;
}
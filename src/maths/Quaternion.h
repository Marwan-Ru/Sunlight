// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <array>
#include <maths/Vector3.h>

class Quaternion
{
public:
    ///
    /// \brief Quaternion Default Constructor (x = 0.0, y=0.0, z =0.0, w =0.0)
    ///
    Quaternion();

    ///
    /// \brief Quaternion Constructor given x,y,z,w values
    /// \param x x value
    /// \param y y value
    /// \param z z value
    /// \param w w value
    ///
    Quaternion(double x, double y, double z, double w);

    ///
    /// \brief Quaternion Construct a Quaternion from an axis and an angle
    /// \param axis Axis of rotation
    /// \param angle Angle of rotation
    ///
    Quaternion(const TVec3d& axis, const double angle);

public:
    double x, y, z, w;
};

///
/// \brief operator * Multiplication of a Quaternion by a scalar
/// \param q Quaternion
/// \param s scalar
/// \return result as a Quaternion
///
Quaternion operator*(const Quaternion& q, double s);

///
/// \brief operator * Multiplication of a Quaternion by a scalar
/// \param s scalar
/// \param q Quaternion
/// \return result as a Quaternion
///
Quaternion operator*(double s, const Quaternion& q);

///
/// \brief operator * Multiplication of a Quaternion by a vector (applies rotation Quaternion to vector)
/// \param q Quaternion
/// \param vec vector
/// \return new vector rotated
///
TVec3d operator*(Quaternion const& q, TVec3d const& vec);

///
/// \brief operator * Multiplication of a Quaternion by a Quaternion
/// \param q1 left side Quaternion
/// \param q2 right side Quaternion
/// \return result as a Quaternion
///
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);

///
/// \brief operator << print Quaternion values
/// \param stream out stream parameter
/// \param q Quaternion
/// \return Output stream of Quaternion q : "q.x ; q.y ; q.z ; q.w"
///
std::ostream& operator<<(std::ostream& stream, const Quaternion& q);

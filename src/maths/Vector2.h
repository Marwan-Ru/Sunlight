// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <iostream>
#include <algorithm>

// 2D vector class.
template< class T > class TVec2
{
public:
	union
	{
		T xy[2];
		struct { T x, y; };
	};

public:
	TVec2(const T x = (T)0, const T y = (T)0);
	TVec2(const T vec[]);

	TVec2 operator+(const TVec2<T>& v) const;
	TVec2 operator-(const TVec2<T>& v) const;
	TVec2 operator-() const;
	TVec2 operator*(const T& v) const;
	TVec2 operator*(const TVec2<T>& v) const;
	TVec2 operator/(const T& v) const;
	TVec2 operator/(const TVec2<T>& v) const;

	bool operator==(const TVec2<T>& v) const;
	bool operator!=(const TVec2<T>& v) const;

	inline operator T* () { return xy; }
	inline operator const T* () const { return xy; }
};

template< class T >	inline TVec2<T>::TVec2(const T x, const T y)
{
	this->x = x;
	this->y = y;
}

template< class T >	inline TVec2<T>::TVec2(const T vec[])
{
	memcpy(xy, vec, 2 * sizeof(T));
}

template< class T >	inline TVec2<T> TVec2<T>::operator+(const TVec2<T>& v) const
{
	return TVec2<T>(x + v.x, y + v.y);
}

template< class T >	inline TVec2<T> TVec2<T>::operator-(const TVec2<T>& v) const
{
	return TVec2<T>(x - v.x, y - v.y);
}

template< class T >	inline TVec2<T> TVec2<T>::operator-() const
{
	return TVec2<T>(-x, -y);
}

template< class T >	inline TVec2<T> TVec2<T>::operator*(const T& v) const
{
	return TVec2<T>(x * v, y * v);
}

template< class T >	inline TVec2<T> TVec2<T>::operator*(const TVec2<T>& v) const
{
	return TVec2<T>(x * v.x, y * v.y);
}

template< class T >	inline TVec2<T> TVec2<T>::operator/(const T& v) const
{
	return TVec2<T>(x / v, y / v);
}

template< class T >	inline TVec2<T> TVec2<T>::operator/(const TVec2<T>& v) const
{
	return TVec2<T>(x / v.x, y / v.y);
}

template< class T >	inline bool TVec2<T>::operator==(const TVec2<T>& v) const
{
	return x == v.x && y == v.y;
}

template< class T >	inline bool TVec2<T>::operator!=(const TVec2<T>& v) const
{
	return x != v.x || y != v.y;
}

template< class T >	inline TVec2<T> operator*(const T& val, const TVec2<T>& vec)
{
	return TVec2<T>(vec.x * val, vec.y * val);
}

template<class T> inline std::ostream& operator<<(std::ostream& os, TVec2<T> const& v)
{
	return os << "(" << std::fixed << v.x << "," << std::fixed << v.y << ")";
}

template<class T> inline std::istream& operator>>(std::istream& is, TVec2<T>& v)
{
	return is >> v.x >> v.y;
}

template<class T> inline TVec2<T> getMinimumCoordinate(TVec2<T>& a, TVec2<T>& b)
{
   T x = a.x < b.x ? a.x : b.x;
   T y = a.y < b.y ? a.y : b.y;

   return TVec2<T>(x, y);
}

template<class T> inline TVec2<T> getMaximumCoordinate(TVec2<T>& a, TVec2<T>& b)
{
   T x = a.x > b.x ? a.x : b.x;
   T y = a.y > b.y ? a.y : b.y;

   return TVec2<T>(x, y);
}

typedef TVec2< int >          TVec2i;
typedef TVec2< float >			TVec2f;
typedef TVec2< double >			TVec2d;

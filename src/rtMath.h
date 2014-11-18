#ifndef RT_MATH_H
#define RT_MATH_H

#include "rtVec3.h"
#include <cmath>

namespace rt
{
	static constexpr float pi = 3.1415926535897932384626f;
	static constexpr float halfPi = pi / 2.0f;
	static constexpr float twoPi = pi * 2.0f;

	rt::Vec3	CrossProduct( const rt::Vec3& vecA, const rt::Vec3& vecB );
	float		DotProduct( const rt::Vec3& vecA, const rt::Vec3& vecB );
}

#endif // RT_MATH_H

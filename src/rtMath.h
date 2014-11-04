#ifndef RT_MATH_H
#define RT_MATH_H

#include "rtVec3.h"

namespace rt
{
	static constexpr float pi = 3.141592653589793238462643383279502884;
	static constexpr float halfPi = pi / 2.0;
	static constexpr float twoPi = pi * 2.0;

	rt::Vec3	CrossProduct( const rt::Vec3& vecA, const rt::Vec3& vecB );
	float		DotProduct( const rt::Vec3& vecA, const rt::Vec3& vecB );
}

#endif // RT_MATH_H

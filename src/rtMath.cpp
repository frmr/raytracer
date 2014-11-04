#include "rtMath.h"

#include <cmath>

rt::Vec3 rt::CrossProduct( const rt::Vec3& vecA, const rt::Vec3& vecB )
{
	return rt::Vec3( vecA.y * vecB.z - vecA.z * vecB.y,
						vecA.z * vecB.x - vecA.x * vecB.z,
						vecA.x * vecB.y - vecA.y * vecB.x );
}

float rt::DotProduct( const rt::Vec3& vecA, const rt::Vec3& vecB )
{
    return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

#include "rtRayTracer.h"
#include "rtMath.h"
#include <limits>

//----------------------------------------------------------------------------
//	Intersects: Checks for intersection between ray and triangle.
//	Returns true if they intersect.
//	Depth is to the factor of the rayVector equals the point of intersection.
//
//	Help with this method was derived from code supplied with:
//		"Physical Based Rendering: From Theory to Implementation"
//		By Matt Pharr and Greg Humphreys
//		ISBN-10: 0123750792
//		ISBN-13: 978-0123750792
//----------------------------------------------------------------------------

bool rt::RayTracer::Triangle::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const
{
	rt::Vec3 s1 = rt::CrossProduct( rayVector, v02 );
	float divisor = rt::DotProduct( s1, v01 );

	if( divisor == 0.0f )
	{
		return false;
	}

	float invDivisor = 1.0f / divisor;
	rt::Vec3 d = rayOrigin - v0;
	float b1 = rt::DotProduct( d, s1 ) * invDivisor;

	if( b1 <= 0.0f || b1 > 1.0f )
	{
		return false;
	}

	rt::Vec3 s2 = rt::CrossProduct( d, v01 );

	float b2 = rt::DotProduct( rayVector, s2 ) * invDivisor;

	if( b2 <= 0.0f || b1 + b2 > 1.0f )
	{
		return false;
	}

	depth = rt::DotProduct( v02, s2 ) * invDivisor;

	return ( depth > 0.0f ) ? true : false;
}

bool rt::RayTracer::Triangle::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, const int reflectionLimit, int reflectionDepth, float rayPower, const rt::Vec3& ambientLight, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	return ShapeHit( rayOrigin, rayVector, depth, normal, reflectionLimit, reflectionDepth, rayPower, ambientLight, lights, shapes, rayColor );
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2 )
	:	Shape(),
		v0( v0 ), v1( v1 ), v2( v2 ),
		v01( ( v1 - v0 ) ), v02( ( v2 - v0 ) ),
        normal( rt::CrossProduct( v01.Unit(), v02.Unit() ).Unit() )
{
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
	:	Shape( ambient, diffuse, specular, shininess ),
		v0( v0 ), v1( v1 ), v2( v2 ),
        v01( ( v1 - v0 ) ), v02( ( v2 - v0 ) ),
        normal( rt::CrossProduct( v01.Unit(), v02.Unit() ).Unit() )
{
}

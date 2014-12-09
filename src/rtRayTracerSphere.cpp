#include "rtRayTracer.h"
#include "rtMath.h"
#include <limits>

//----------------------------------------------------------------------------
//	Intersects: Checks for intersection between ray and sphere.
//	Returns true if they intersect.
//	Depth is to the factor of the rayVector equals the point of intersection.
//
//	Help with this method was derived from code in:
//		"Realistic Ray Tracing Second Edition"
//		By Peter Shirley and R. Keith Morley
//		ISBN-10: 1568814615
//		ISBN-13: 978-1568814612
//----------------------------------------------------------------------------

bool rt::RayTracer::Sphere::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const
{
	rt::Vec3 temp = rayOrigin - origin;

	float a = rt::DotProduct( rayVector, rayVector );
	float b = 2.0f * rt::DotProduct( rayVector, temp );
	float c = rt::DotProduct( temp, temp ) - radiusSquared;
	float discriminant = ( b * b ) - ( 4.0f * a * c );

	if ( discriminant > 0.0f )
	{
		discriminant = sqrt( discriminant );
		depth = ( -b - discriminant ) / ( 2.0f * a );
		return ( depth > 0.0f ) ? true : false;
	}
	else
	{
		return false;
	}
}

//-------------------------------------------
// Hit: Computes the suface normal at the
// point of intersection and calls ShapeHit.
//-------------------------------------------

bool rt::RayTracer::Sphere::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, const int reflectionLimit, int reflectionDepth, float rayPower, const rt::Vec3& ambientLight, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	rt::Vec3 intersection = rayOrigin + rayVector * depth;
	rt::Vec3 normal = ( intersection - origin ).Unit();

	return ShapeHit( rayOrigin, rayVector, depth, normal, reflectionLimit, reflectionDepth, rayPower, ambientLight, lights, shapes, rayColor );
}

rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius )
	:	Shape(),
		origin( origin ),
		radius( radius ),
		radiusSquared( radius * radius )
{
}

rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
	:	Shape( ambient, diffuse, specular, shininess ),
		origin( origin ),
		radius( radius ),
		radiusSquared( radius * radius )
{
}

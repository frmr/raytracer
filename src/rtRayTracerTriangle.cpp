#include "rtRayTracer.h"
#include "rtMath.h"

bool rt::RayTracer::Triangle::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const
{
//	rt::Vec3 rayPlaneVector = v0 - rayOrigin;
//
//	rt::Vec3 s1 = Cross( ray.d, e2 );
//	float divisor = Dot(s1, e1);
//	if (divisor == 0.0f) return false;

	return false;
}

bool rt::RayTracer::Triangle::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
    return false;
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		v0( v0 ), v1( v1 ), v2( v2 ),
        v01( ( v1 - v0 ).Unit() ), v02( ( v2 - v0 ).Unit() ),
        normal( rt::CrossProduct( v01, v02 ) )
{
}

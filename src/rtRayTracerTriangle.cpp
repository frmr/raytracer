#include "rtRayTracer.h"
#include "rtMath.h"

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

	if( b1 < 0.0f || b1 > 1.0f )
	{
		return false;
	}

	rt::Vec3 s2 = rt::CrossProduct( d, v01 );

	float b2 = rt::DotProduct( rayVector, s2 ) * invDivisor;

	if( b2 < 0.0 || b1 + b2 > 1.0f )
	{
		return false;
	}

	depth = rt::DotProduct( v02, s2 ) * invDivisor;

	return ( depth > 0.0f ) ? true : false;
	{

	}

//	if( r.trunc( t ) )
//	{
//		i.set(this,ray.getPointAt(t),normal,b1,b2);
//		return true;
//	}
}

bool rt::RayTracer::Triangle::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	rt::Vec3 intersection = rayOrigin + rayVector * depth;

	for ( auto light : lights )
	{
		rt::Vec3 lightVec = ( light.origin - intersection ).Unit();
		float dotLight = rt::DotProduct( normal, lightVec );
		if ( dotLight > 0.0f )
		{
			bool occluded = false;
			for ( const auto& shape : shapes )
			{
				if ( !( id == shape->id ) )
				{
					float tempDepth;
					if ( shape->Intersects( intersection, lightVec, tempDepth ) )
					{
						occluded = true;
						break;
					}
				}

			}
			if ( !occluded )
			{
				rayColor += color * rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
			}
		}
	}
    return true;
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		v0( v0 ), v1( v1 ), v2( v2 ),
        //v01( ( v1 - v0 ).Unit() ), v02( ( v2 - v0 ).Unit() ),
        v01( ( v1 - v0 ) ), v02( ( v2 - v0 ) ),
        normal( rt::CrossProduct( v01.Unit(), v02.Unit() ).Unit() )
{
}

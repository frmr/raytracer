#include "rtRayTracer.h"
#include "rtMath.h"

bool rt::RayTracer::Sphere::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const
{
	rt::Vec3 temp = rayOrigin - origin;

	float a = rt::DotProduct( rayVector, rayVector );
	float b = 2.0f * rt::DotProduct( rayVector, temp );
	float c = rt::DotProduct( temp, temp ) - radiusSquared;
	float discriminant = ( b * b ) - ( 4.0f * a * c );

	if ( discriminant > 0.0f )
	{
		discriminant = sqrt( discriminant );
		t = ( -b - discriminant ) / ( 2.0f * a );
		return true;
	}
	else
	{
		return false;
	}
}

bool rt::RayTracer::Sphere::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	rt::Vec3 intersection = rayOrigin + rayVector * t;
	rt::Vec3 normal = ( intersection - origin ).Unit();

	for ( auto light : lights )
	{
		float dotLight = rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
		if ( dotLight > 0.0f )
		{
			rt::Vec3 lightVec = ( light.origin - intersection ).Unit();
			bool occluded = false;
			for ( const auto& shape : shapes )
			{
				if ( !( id == shape->id ) )
				{
					float tempT;
					if ( shape->Intersects( intersection, lightVec, tempT ) )
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

rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		origin( origin ), vectorLengthSquared( origin.Length() * origin.Length() ), radius( radius ), radiusSquared( radius * radius )
{
}
#include "rtRayTracer.h"
#include "rtMath.h"
#include <cmath>

#include <iostream>
using namespace std;

rt::RayTracer::Light::Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
	:	origin( origin ), color( color ), intensity( intensity )
{

}

int rt::RayTracer::Shape::idCounter = 0;

rt::RayTracer::Shape::Shape( const rt::Vec3 color, const float reflectivity )
	:	id( idCounter++ ), color( color ), reflectivity( reflectivity )
{
}

float rt::RayTracer::Triangle::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector ) const
{
	return -1.0f;
}

bool rt::RayTracer::Triangle::CheckIntersection( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float rayPower, const vector<rt::RayTracer::Light>& lights, const vector<shared_ptr<rt::RayTracer::Shape>>& shapes, rt::Vec3& rayColor ) const
{
	//get plane intersection point
	//check if point is within triangle
    return true;
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		vert0( vert0 ), vert1( vert1 ), vert2( vert2 ),
        vec01( ( vert1 - vert0 ).Unit() ), vec02( ( vert2 - vert0 ).Unit() ),
        normal( rt::CrossProduct( vec01, vec02 ) )
{
}

float rt::RayTracer::Sphere::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector ) const
{
	rt::Vec3 temp = rayOrigin - origin;

	float a = rt::DotProduct( rayVector, rayVector );
	float b = 2.0f * rt::DotProduct( rayVector, temp );
	float c = rt::DotProduct( temp, temp ) - radiusSquared;
	float discriminant = ( b * b ) - ( 4.0f * a * c );

	if ( discriminant > 0.0f )
	{
		discriminant = sqrt( discriminant );
		return ( -b - discriminant ) / ( 2.0f * a );
	}
	else
	{
		return -1.0f;
	}
}

bool rt::RayTracer::Sphere::CheckIntersection( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float rayPower, const vector<rt::RayTracer::Light>& lights, const vector<shared_ptr<rt::RayTracer::Shape>>& shapes, rt::Vec3& rayColor ) const
{
	float t = Intersects( rayOrigin, rayVector );

	if ( t )
	{
		rt::Vec3 intersection = rayVector * t;
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
						if ( shape->Intersects( intersection, lightVec ) > 0.0f )
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
	else
	{
		return false;
	}
}

rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		origin( origin ), vectorLengthSquared( origin.Length() * origin.Length() ), radius( radius ), radiusSquared( radius * radius )
{
}

bool rt::RayTracer::AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
{
	lights.push_back( Light( origin, color, intensity ) );
	return true;
}

bool rt::RayTracer::AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
{
	shapes.push_back( std::make_shared<Triangle>( vert0, vert1, vert2, color, reflectivity ) );
	return true;
}

bool rt::RayTracer::AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
{
	shapes.push_back( std::make_shared<Sphere>( origin, radius, color, reflectivity ) );
	return true;
}

rt::RayTracer::rtError rt::RayTracer::Sample( const rt::Vec3& rayVector, rt::Vec3& sampleColor ) const
{
	for ( auto shape : shapes )
	{
		rt::Vec3 rayOrigin;
		shape->CheckIntersection( rayOrigin, rayVector, 1.0f, lights, shapes, sampleColor );
	}

	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
{
}

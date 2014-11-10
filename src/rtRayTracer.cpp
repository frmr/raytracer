#include "rtRayTracer.h"
#include "rtMath.h"
#include <cmath>

#include <iostream>
using namespace std;

rt::RayTracer::Light::Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
	:	origin( origin ), color( color ), intensity( intensity )
{

}

rt::RayTracer::Shape::Shape( const rt::Vec3 color, const float reflectivity )
	:	color( color ), reflectivity( reflectivity )
{
}

bool rt::RayTracer::Triangle::CheckIntersection( const rt::Vec3& rayVector, const float rayPower, const vector<rt::RayTracer::Light>& lights, rt::Vec3& rayColor ) const
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

bool rt::RayTracer::Sphere::CheckIntersection( const rt::Vec3& rayVector, const float rayPower, const vector<rt::RayTracer::Light>& lights, rt::Vec3& rayColor ) const
{
//	float dotOriginRay = rt::DotProduct( origin, rayVector );
//
//	float b = rt::DotProduct( rayVector, origin );
//	float c = rt::DotProduct( origin, origin ) - radiusSquared;
//	float deltabis = b * b - c;
//
//	//cout << deltabis << endl;
//
//	if (deltabis < 0 || deltabis > 1 )
//	{
//		return false;
//	}
//
//	rayColor = color;
//
//	return true;

	rt::Vec3 temp = origin.Reverse();

	float a = rt::DotProduct( rayVector, rayVector );
	float b = 2.0f * rt::DotProduct( rayVector, temp );
	float c = rt::DotProduct( temp, temp ) - radiusSquared;

	// this is the term under the square-root
	// (see solution for quadratc equation)
	float discriminant = ( b * b ) - ( 4.0f * a * c );
	//first check if ray intersects sphere

	if ( discriminant > 0.0f )
	{
		discriminant = sqrt( discriminant );
		float t = ( -b - discriminant ) / ( 2 * a );

		// check for valid interval
		if ( t < 0.0f ) t = ( -b + discriminant ) / ( 2.0f * a );
		if (t < 0.0f || t > 100.0f) return false; //TODO: tmin and tmax

		rt::Vec3 intersection = rayVector * t;
		rt::Vec3 normal = ( intersection - origin ).Unit();

		for ( auto light : lights )
		{
			float dotLight = rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
			if ( dotLight > 0.0f )
			{
				rayColor += color * rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
			}

		}


		//rayColor = color;
		return true;
	}
	return false;


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
	entities.push_back( std::make_shared<Triangle>( vert0, vert1, vert2, color, reflectivity ) );
	return true;
}

bool rt::RayTracer::AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
{
	entities.push_back( std::make_shared<Sphere>( origin, radius, color, reflectivity ) );
	return true;
}

rt::RayTracer::rtError rt::RayTracer::Sample( const float sampleAngleX, const float sampleAngleY, rt::Vec3& sampleColor ) const
{
	//convert angle to vector
	rt::Vec3 rayVector( sin( sampleAngleX ), tan( sampleAngleY ), cos( sampleAngleX ) );
	//rt::Vec3 rayVector( distToProjPlane * tan( sampleAngleX ), distToProjPlane * tan( sampleAngleY ), distToProjPlane );
	rayVector.Unit(); //might not be necessary

	//cout << rayVector.x << " " << rayVector.y << " " << rayVector.z << endl;

	return Sample( rayVector, sampleColor );
}

rt::RayTracer::rtError rt::RayTracer::Sample( const rt::Vec3 rayVector, rt::Vec3& sampleColor ) const
{
	for ( auto Shape : entities )
	{
		Shape->CheckIntersection( rayVector, 1.0f, lights, sampleColor );
	}

	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
{
}

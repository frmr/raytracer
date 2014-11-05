#include "rtRayTracer.h"
#include "rtMath.h"
#include <cmath>

rt::RayTracer::Light::Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
	:	origin( origin ), color( color ), intensity( intensity )
{

}

rt::RayTracer::Entity::Entity( const rt::Vec3 color, const float reflectivity )
	:	color( color ), reflectivity( reflectivity )
{
}

bool rt::RayTracer::Polygon::CheckIntersection( const rt::Vec3& rayVector, rt::Vec3& rayColor, float& rayPower ) const
{
    return true;
}

rt::RayTracer::Polygon::Polygon( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
	:	Entity( color, reflectivity ),
		vert0( vert0 ), vert1( vert1 ), vert2( vert2 ),
        vec01( ( vert1 - vert0 ).Unit() ), vec02( ( vert2 - vert0 ).Unit() ),
        normal( rt::CrossProduct( vec01, vec02 ) )
{
}

bool rt::RayTracer::Sphere::CheckIntersection( const rt::Vec3& rayVector, rt::Vec3& rayColor, float& rayPower ) const
{
	//if (  )
	{
		//for each light
			//check occlusions
		//bound ray, check other spheres
		rayColor = color;
		return true;
	}
	return false;
}


rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
	:	Entity( color, reflectivity ),
		origin( origin ), radius( radius )
{
}

bool rt::RayTracer::AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
{
	lights.push_back( Light( origin, color, intensity ) );
	return true;
}

bool rt::RayTracer::AddPolygon( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
{
	entities.push_back( std::make_shared<Polygon>( vert0, vert1, vert2, color, reflectivity ) );
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
	rt::Vec3 rayVector( cos( sampleAngleX ), sin( sampleAngleX ), tan( sampleAngleY ) );
	rayVector.Unit(); //might not be necessary

	return Sample( rayVector, sampleColor );
}

rt::RayTracer::rtError rt::RayTracer::Sample( const rt::Vec3 rayVector, rt::Vec3& sampleColor ) const
{
	for ( auto entity : entities )
	{

	}

	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
{
}

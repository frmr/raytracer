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

bool rt::RayTracer::Polygon::CheckIntersection( const rt::Vec3& rayVector, const float rayPower, rt::Vec3& rayColor ) const
{
	//get plane intersection point
	//check if point is within triangle
    return true;
}

rt::RayTracer::Polygon::Polygon( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
	:	Entity( color, reflectivity ),
		vert0( vert0 ), vert1( vert1 ), vert2( vert2 ),
        vec01( ( vert1 - vert0 ).Unit() ), vec02( ( vert2 - vert0 ).Unit() ),
        normal( rt::CrossProduct( vec01, vec02 ) )
{
}

bool rt::RayTracer::Sphere::CheckIntersection( const rt::Vec3& rayVector, const float rayPower, rt::Vec3& rayColor ) const
{
	float dotOriginRay = rt::DotProduct( origin, rayVector );

	//Not necessary since the camera can't move, so all spheres are probably visible
//	if ( dotOriginRay < 0.0f )
//	{
//		return false;
//	}

//	Vector3d oc = m_Center - ray.origin;
//
//	double tca = Dot(oc, ray.dir);
//	if (tca < 0)
//	{
//		// points away from the sphere
//		return false;
//	}

//	if ( rayVector.x <= 0.0f )
//	{
//		rayColor = rt::Vec3( 1.0f, 0.0f, 0.0f );
//		return true;
//	}
//	else
//	{
//		rayColor = rt::Vec3( 0.0f, 0.0f, 1.0f );
//		return true;
//	}


	double l2hc = ( radiusSquared - vectorLengthSquared ) + ( dotOriginRay * dotOriginRay );
	if ( l2hc > 0 )
	{
		rayColor = color;
//		double t = tca - sqrt(l2hc);
//		// calculate position
//		pos = ray.origin + ray.dir * t;
//		// radius ray
//		normal = pos - m_Center;
//		normal.Normalize();
		return true;
	}
	return false;
}

rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
	:	Entity( color, reflectivity ),
		origin( origin ), vectorLengthSquared( origin.Length() * origin.Length() ), radius( radius ), radiusSquared( radius * radius )
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
	rt::Vec3 rayVector( sin( sampleAngleX ), tan( sampleAngleY ), sin( sampleAngleX ) );
	//rt::Vec3 rayVector( distToProjPlane * tan( sampleAngleX ), distToProjPlane * tan( sampleAngleY ), distToProjPlane );
	rayVector.Unit(); //might not be necessary

	return Sample( rayVector, sampleColor );
}

rt::RayTracer::rtError rt::RayTracer::Sample( const rt::Vec3 rayVector, rt::Vec3& sampleColor ) const
{
	for ( auto entity : entities )
	{
		entity->CheckIntersection( rayVector, 1.0f, sampleColor );
	}

	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
{
}

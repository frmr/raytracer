#include "rtRayTracer.h"
#include "rtMath.h"

int rt::RayTracer::Shape::idCounter = 0;

bool rt::RayTracer::Shape::UpdateColor( const rt::Vec3& rayVector, const rt::Vec3& lightVector, const rt::Vec3& surfaceNormal, rt::Vec3& rayColor ) const
{
	rayColor += diffuse * rt::DotProduct( surfaceNormal, lightVector );
	return true;
}

rt::RayTracer::Shape::Shape()
	:	id( idCounter++ ),
		ambient( 0.5f, 0.5f, 0.5f ),
		diffuse( 0.5f, 0.5f, 0.5f ),
		specular( 0.5f, 0.5f, 0.5f ),
		shininess( 0.0f )
{
}

rt::RayTracer::Shape::Shape( const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
	:	id( idCounter++ ),
		ambient( ambient ),
		diffuse( diffuse ),
		specular( specular ),
		shininess( shininess )
{
}

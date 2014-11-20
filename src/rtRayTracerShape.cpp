#include "rtRayTracer.h"

int rt::RayTracer::Shape::idCounter = 0;

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

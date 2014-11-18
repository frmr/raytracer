#include "rtRayTracer.h"

int rt::RayTracer::Shape::idCounter = 0;

rt::RayTracer::Shape::Shape( const rt::Vec3 color, const float reflectivity )
	:	id( idCounter++ ),
		color( color ),
		reflectivity( reflectivity )
{
}

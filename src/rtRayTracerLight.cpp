#include "rtRayTracer.h"

rt::RayTracer::Light::Light( const rt::Vec3 origin, const rt::Vec3 color )
	:	origin( origin ),
		color( color )
{
}

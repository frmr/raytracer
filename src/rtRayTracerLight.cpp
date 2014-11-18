#include "rtRayTracer.h"

rt::RayTracer::Light::Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
	:	origin( origin ),
		color( color ),
		intensity( intensity )
{
}

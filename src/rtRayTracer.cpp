#include "rtRayTracer.h"

bool rt::RayTracer::AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
{
	lights.push_back( Light( origin, color, intensity ) );
	return true;
}

bool rt::RayTracer::AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color )
{
	spheres.push_back( rt::Sphere)
	return true;
}

rt::RayTracer::rtError rt::RayTracer::Sample( rt::Vec3& sampleColor ) const
{
	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
{
}

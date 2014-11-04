#ifndef RT_RAY_TRACER_H
#define RT_RAY_TRACER_H

#include <vector>

#include "rtVec3.h"

using std::vector;

namespace rt
{
	class RayTracer
	{
	private:
		enum class rtError
		{
			SUCCESS
		};

		class Light
		{
		public:
			const rt::Vec3 origin;
			const rt::Vec3 color;
			const float intensity;

		public:
			Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
				: origin( origin ), color( color ), intensity( intensity ){}
		};

		class Sphere
		{
		public:
			Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color );
		};

	private:
		vector<Light>		lights;
		vector<Sphere>	spheres;

	public:
		bool	AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity );
		bool	AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color );
		rtError Sample( rt::Vec3& sampleColor ) const;

	public:
		RayTracer();
	};
}


#endif // RT_RAY_TRACER_H

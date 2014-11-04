#ifndef RT_RAY_TRACER_H
#define RT_RAY_TRACER_H

#include <vector>

#include "rtSphere.h"
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

	private:


	public:
		bool	AddLight( const rt::Vec3& origin, const rt::Ve3& color, const float intensity );
		bool	AddSphere( const rt::Vec3& origin, const double radius, const rt::Vec3& color );
		rtError Sample( const float sampleAngleX, const float sampleAngleY, float& sampleRed, float& sampleGreen, float& sampleBlue ) const;

	public:
		RayTracer();
	};
}


#endif // RT_RAY_TRACER_H

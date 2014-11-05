#ifndef RT_RAY_TRACER_H
#define RT_RAY_TRACER_H

#include <memory>
#include <vector>

#include "rtVec3.h"

using std::shared_ptr;
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
			Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity );
		};




		class Entity
		{
		public:
			const rt::Vec3	color;
			const float		reflectivity;

		public:
			virtual bool CheckIntersection( const rt::Vec3& rayVector, const float rayPower, rt::Vec3& rayColor ) const = 0;

		protected:
			Entity( const rt::Vec3 color, const float reflectivity );
		};




		class Polygon : public Entity
		{
		public:
			const rt::Vec3 vert0;
			const rt::Vec3 vert1;
			const rt::Vec3 vert2;
			const rt::Vec3 vec01;
			const rt::Vec3 vec02;
			const rt::Vec3 normal;

		public:
			bool CheckIntersection( const rt::Vec3& rayVector, const float rayPower, rt::Vec3& rayColor ) const;

		public:
			Polygon( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity );
		};




		class Sphere : public Entity
		{
		public:
			const rt::Vec3	origin;
			const float		vectorLengthSquared;
			const float		radius;
			const float		radiusSquared;

		public:
			bool CheckIntersection( const rt::Vec3& rayVector, const float rayPower, rt::Vec3& rayColor ) const;

		public:
			Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity );
		};




	private:
		vector<Light>				lights;
		vector<shared_ptr<Entity>>	entities; //use unique_ptr in C++14

	public:
		bool	AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity );
		bool	AddPolygon( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity );
		bool	AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity );
		rtError Sample( const float sampleAngleX, const float sampleAngleY, rt::Vec3& sampleColor ) const;
		rtError Sample( const rt::Vec3 rayVector, rt::Vec3& sampleColor ) const;

	public:
		RayTracer();
	};
}


#endif // RT_RAY_TRACER_H

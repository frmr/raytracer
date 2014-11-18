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




		class Shape
		{
		private:
			static int		idCounter;
		public:
			const int		id;
			const rt::Vec3	color;
			const float		reflectivity;

		public:
			virtual bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const = 0;
			virtual bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const = 0;

		protected:
			Shape( const rt::Vec3 color, const float reflectivity );
		};




		class Sphere : public Shape
		{
		public:
			const rt::Vec3	origin;
			const float		vectorLengthSquared;
			const float		radius;
			const float		radiusSquared;

		public:
			bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const;
			bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const;

		public:
			Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity );
		};




		class Triangle : public Shape
		{
		public:
			const rt::Vec3 v0;
			const rt::Vec3 v1;
			const rt::Vec3 v2;
			const rt::Vec3 v01;
			const rt::Vec3 v02;
			const rt::Vec3 normal;

		public:
			bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const;
			bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const;

		public:
			Triangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity );
		};




	private:
		vector<Light>				lights;
		vector<shared_ptr<Shape>>	shapes; //use unique_ptr in C++14

	public:
		bool	AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity );
		bool	AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity );
		bool	AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity );
		rtError Sample( const rt::Vec3& rayVector, rt::Vec3& sampleColor ) const;

	public:
		RayTracer();
	};
}


#endif // RT_RAY_TRACER_H

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

		public:
			Light( const rt::Vec3 origin, const rt::Vec3 color );
		};




		class Shape
		{
		private:
			static int		idCounter;
		public:
			const int		id;
			const rt::Vec3	ambient;
			const rt::Vec3	diffuse;
			const rt::Vec3	specular;
			const float		shininess;

		protected:
			void			SpawnReflectionRay( const rt::Vec3& intersection, const rt::Vec3& rayVector, const rt::Vec3& normal, const float rayPower, const vector<Light>& lightsRef, const vector<shared_ptr<Shape>>& shapesRef, rt::Vec3& rayColor ) const;
			bool			UpdateColor( const rt::Vec3& rayVector, const rt::Vec3& lightVector, const rt::Vec3& surfaceNormal, rt::Vec3& rayColor ) const; //TODO: Remove?

		public:
			virtual bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const = 0;
			virtual bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const = 0;

		protected:
			Shape();
			Shape( const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );
		};




		class Sphere : public Shape
		{
		public:
			const rt::Vec3	origin;
			const float		radius;
			const float		radiusSquared;

		public:
			bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const;
			bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const;

		public:
			Sphere( const rt::Vec3 origin, const float radius );
			Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );
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
			bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const;
			bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const;

		public:
			Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2 );
			Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );
		};




	private:
		vector<Light>				lights;
		vector<shared_ptr<Shape>>	shapes;

	public:
		bool	AddLight( const rt::Vec3 origin, const rt::Vec3 color );

		bool	AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2 );
		bool	AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );

		bool	AddSphere( const rt::Vec3 origin, const float radius );
		bool	AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );

		rtError Sample( const rt::Vec3& rayVector, rt::Vec3& sampleColor ) const;

	public:
		RayTracer();
	};
}


#endif // RT_RAY_TRACER_H

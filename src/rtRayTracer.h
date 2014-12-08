#ifndef RT_RAY_TRACER_H
#define RT_RAY_TRACER_H

#include <memory>
#include <vector>

#include "rtVec3.h"

using std::shared_ptr;
using std::vector;

namespace rt
{
	//-----------------------------------------------------------------------------------
	//	RayTracer: Class that stores shapes in the scene and initiates sampling of them.
	//	Also stores lights and controls ambient lighting.
	//-----------------------------------------------------------------------------------
	
	class RayTracer
	{
	private:




		enum class rtError
		{
			SUCCESS
		};



		//-------------------------------------------------------
		//	Light: Class that stores simple lighting properties.
		//-------------------------------------------------------
		
		class Light
		{
		public:
			const rt::Vec3 origin;
			const rt::Vec3 color;

		public:
			Light( const rt::Vec3 origin, const rt::Vec3 color );
		};



		//-----------------------------------------------------
		//	Shape: Base class of Sphere and Triangle classes.
		//	Stores material properties used for Phong shading.
		//-----------------------------------------------------
		
		class Shape
		{
		private:
			static int			idCounter;
		public:
			const int			id;
			const rt::Vec3		ambient;
			const rt::Vec3		diffuse;
			const rt::Vec3		specular;
			const float			shininess;

		private:
			
			//------------------------------------------------------------
			//	SpawnReflectionRay: Computer the ray of reflection at the
			//	point of intersection and sample the first shape it hits.
			//	Outputs final color to rayColor.
			//------------------------------------------------------------
			
			void			SpawnReflectionRay( const rt::Vec3& 					intersection,
												const rt::Vec3& 					rayVector,
												const rt::Vec3& 					normal,
												const int 							reflectionLimit,
												int 								reflectionDepth,
												const float 						rayPower,
												const rt::Vec3& 					ambientLight,
												const vector<Light>& 				lightsRef,
												const vector<shared_ptr<Shape>>&	shapesRef,
												rt::Vec3&							rayColor ) const;

		protected:
			
			//-----------------------------------------------------------------------------------------------------
			//	ShapeHit: Generalised method for performing coloring calculations in the event of a ray collision.
			//	This method in the base class is called by all derived classes after they have locally determined
			//	the normal at the point of intersection. Outputs final color to rayColor.
			//-----------------------------------------------------------------------------------------------------
			
			bool			ShapeHit( 	const rt::Vec3& 					rayOrigin,
										const rt::Vec3& 					rayVector,
										const float 						depth,
										const rt::Vec3& 					intersectionNormal,
										const int 							reflectionLimit,
										int 								reflectionDepth,
										float 								rayPower,
										const rt::Vec3& 					ambientLight,
										const vector<Light>& 				lights,
										const vector<shared_ptr<Shape>>&	shapes,
										rt::Vec3& 							rayColor ) const;

		public:
			
			//-------------------------------------------------------------------------------------
			//	Intersects: Pure method, returns true if incoming ray intersects shape.
			//	Depth is set to the factor of rayVector that equates to the point of intersection.
			//-------------------------------------------------------------------------------------
			
			virtual bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const = 0;

			//-------------------------------------------------------------------------------------
			//	Hit: Pure method, computes normal at the point of intersection and calls ShapeHit.
			//	Outputs final color to rayColor.
			//-------------------------------------------------------------------------------------
			
			virtual bool	Hit( 	const rt::Vec3&						rayOrigin,
									const rt::Vec3&						rayVector,
									const float 						depth,
									const int 							reflectionLimit,
									int 								reflectionDepth,
									float 								rayPower,
									const rt::Vec3&						ambientLight,
									const vector<Light>& 				lights,
									const vector<shared_ptr<Shape>>&	shapes,
									rt::Vec3& 							rayColor ) const = 0;

		protected:
			Shape();
			Shape( const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );
		};




		//-------------------------------------------------------------------------------
		//	Sphere: Derived class of shape, implements sphere-specific methods of shape.
		//-------------------------------------------------------------------------------
		
		class Sphere : public Shape
		{
		public:
			const rt::Vec3	origin;
			const float		radius;
			const float		radiusSquared;

		public:
			
			//-------------------------------------------------------------------------------------
			//	Intersects: Checks for intersection with incoming ray.
			//	Returns true if the ray intersects.
			//	Depth is set to the factor of rayVector that equates to the point of intersection.
			//-------------------------------------------------------------------------------------
			
			bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const;
			
			//------------------------------------------------------------------------
			//	Hit: Computes normal at the point of intersection and calls ShapeHit.
			//	Outputs final color to rayColor.
			//------------------------------------------------------------------------
			
			bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, const int reflectionLimit, int reflectionDepth, float rayPower, const rt::Vec3& ambientLight, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const;

		public:
			Sphere( const rt::Vec3 origin, const float radius );
			Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );
		};



		//-----------------------------------------------------------------------------------
		//	Triangle: Derived class of shape, implements triangle-specific methods of shape.
		//-----------------------------------------------------------------------------------

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
			
			//-------------------------------------------------------------------------------------
			//	Intersects: Checks for intersection with incoming ray.
			//	Returns true if the ray intersects.
			//	Depth is set to the factor of rayVector that equates to the point of intersection.
			//-------------------------------------------------------------------------------------
			
			bool	Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const;
			
			//-------------------------------------------------------
			//	Hit: Passes pre-computed surface normal to ShapeHit.
			//	Outputs final color to rayColor.
			//-------------------------------------------------------
			
			bool	Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, const int reflectionLimit, int reflectionDepth, float rayPower, const rt::Vec3& ambientLight, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const;

		public:
			Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2 );
			Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );
		};




	private:
		rt::Vec3					ambientLight;
		vector<Light>				lights;
		vector<shared_ptr<Shape>>	shapes;

	public:
		bool	AddLight( const rt::Vec3 origin, const rt::Vec3 color );

		bool	AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2 );
		bool	AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );

		bool	AddSphere( const rt::Vec3 origin, const float radius );
		bool	AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess );

		rtError Sample( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const int reflectionLimit, rt::Vec3& sampleColor ) const;

		void	SetAmbientLight( const rt::Vec3& ambient );

	public:
		RayTracer();
	};
}


#endif // RT_RAY_TRACER_H

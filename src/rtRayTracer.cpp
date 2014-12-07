#include "rtRayTracer.h"
#include "rtMath.h"
#include <cmath>

#include <iostream>
#include <limits>

bool rt::RayTracer::AddLight( const rt::Vec3 origin, const rt::Vec3 color )
{
	lights.push_back( Light( origin, color ) );
	return true;
}

bool rt::RayTracer::AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2 )
{
	shapes.push_back( std::make_shared<Triangle>( vert0, vert1, vert2 ) );
	return true;
}

bool rt::RayTracer::AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
{
	shapes.push_back( std::make_shared<Triangle>( vert0, vert1, vert2, ambient, diffuse, specular, shininess ) );
	return true;
}

bool rt::RayTracer::AddSphere( const rt::Vec3 origin, const float radius )
{
	shapes.push_back( std::make_shared<Sphere>( origin, radius ) );
	return true;
}

bool rt::RayTracer::AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
{
	shapes.push_back( std::make_shared<Sphere>( origin, radius, ambient, diffuse, specular, shininess ) );
	return true;
}

rt::RayTracer::rtError rt::RayTracer::Sample( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, rt::Vec3& sampleColor ) const
{
	float				closestDepth = std::numeric_limits<float>::max();
	shared_ptr<Shape>	closestShape = nullptr;

	//find closest intersection
	for ( auto shape : shapes )
	{
		float tempDepth;
		if ( shape->Intersects( rayOrigin, rayVector, tempDepth ) )
		{
			if ( tempDepth < closestDepth )
			{
				closestDepth = tempDepth;
				closestShape = shape;
			}
		}
	}

	sampleColor = Vec3();

	//cast ray to closest shape
	if ( closestShape != nullptr )
	{
		closestShape->Hit( rayOrigin, rayVector, closestDepth, 1.0f, lights, shapes, sampleColor );
	}

	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
	:	depthOfField( false ),
		focalDepth( 10.0f )
{
}

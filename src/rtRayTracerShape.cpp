#include "rtRayTracer.h"
#include "rtMath.h"

#include <limits>
#include <vector>

using std::vector;

int rt::RayTracer::Shape::idCounter = 0;

void rt::RayTracer::Shape::SpawnReflectionRay( const rt::Vec3& intersection, const rt::Vec3& rayVector, const rt::Vec3& normal, const float rayPower, const vector<Light>& lightsRef, const vector<shared_ptr<Shape>>& shapesRef, rt::Vec3& rayColor ) const
{
	rt::Vec3 reflectedRayVector = rayVector - normal * 2.0f * rt::DotProduct( rayVector, normal );

	float				closestDepth = std::numeric_limits<float>::max();
	shared_ptr<Shape>	closestShape = nullptr;

	//find closest intersection
	for ( auto shape : shapesRef )
	{
		if ( shape->id != id )
		{
			float tempDepth;
			if ( shape->Intersects( intersection, reflectedRayVector, tempDepth ) )
			{
				if ( tempDepth < closestDepth )
				{
					closestDepth = tempDepth;
					closestShape = shape;
				}
			}
		}

	}

	//cast ray to closest shape
	if ( closestShape != nullptr )
	{
		closestShape->Hit( intersection, reflectedRayVector, closestDepth, rayPower, lightsRef, shapesRef, rayColor );
	}
}

bool rt::RayTracer::Shape::UpdateColor( const rt::Vec3& rayVector, const rt::Vec3& lightVector, const rt::Vec3& surfaceNormal, rt::Vec3& rayColor ) const
{
	rayColor += diffuse * rt::DotProduct( surfaceNormal, lightVector );
	return true;
}

rt::RayTracer::Shape::Shape()
	:	id( idCounter++ ),
		ambient( 0.5f, 0.0f, 0.0f ),
		diffuse( 0.5f, 0.5f, 0.5f ),
		specular( 0.5f, 0.5f, 0.5f ),
		shininess( 0.0f )
{
}

rt::RayTracer::Shape::Shape( const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
	:	id( idCounter++ ),
		ambient( ambient ),
		diffuse( diffuse ),
		specular( specular ),
		shininess( shininess )
{
}

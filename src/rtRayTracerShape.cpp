#include "rtRayTracer.h"
#include "rtMath.h"

#include <limits>
#include <vector>

using std::vector;

int rt::RayTracer::Shape::idCounter = 0;

void rt::RayTracer::Shape::SpawnReflectionRay( const rt::Vec3& intersection, const rt::Vec3& rayVector, const rt::Vec3& normal, const int reflectionLimit,
												int reflectionDepth, const float rayPower, const rt::Vec3& ambientLight, const vector<Light>& lightsRef,
												const vector<shared_ptr<Shape>>& shapesRef, rt::Vec3& rayColor ) const
{
	//Compute vector of ray reflected at the point of intersection
	rt::Vec3 reflectedRayVector = rayVector - normal * 2.0f * rt::DotProduct( rayVector, normal );

	float				closestDepth = std::numeric_limits<float>::max();
	shared_ptr<Shape>	closestShape = nullptr;

	//Find closest intersection with a shape
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

	//Cast ray to closest shape
	if ( closestShape != nullptr )
	{
		closestShape->Hit( intersection, reflectedRayVector, closestDepth, reflectionLimit, ++reflectionDepth, rayPower, ambientLight, lightsRef, shapesRef, rayColor );
	}
}

bool rt::RayTracer::Shape::ShapeHit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, const rt::Vec3& intersectionNormal,
										const int reflectionLimit, int reflectionDepth, float rayPower, const rt::Vec3& ambientLight,
										const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	rt::Vec3 intersection = rayOrigin + rayVector * depth;

	rt::Vec3 ambientAddition = ambientLight * ambient;
	rt::Vec3 diffuseAddition;
	rt::Vec3 specularAddition;

	//Compute lighting at the point of intersection
	for ( auto light : lights )
	{
		rt::Vec3 lightVector = ( light.origin - intersection ).Unit();
		float dotLight = rt::DotProduct( intersectionNormal, lightVector );
		if ( dotLight > 0.0f )
		{
			bool occluded = false;
			for ( const auto& shape : shapes )
			{
				if ( !( id == shape->id ) )
				{
					float tempDepth;
					if ( shape->Intersects( intersection, lightVector, tempDepth ) )
					{
						occluded = true;
						break;
					}
				}

			}
			if ( !occluded )
			{
				diffuseAddition += light.color * diffuse * dotLight;
				rt::Vec3 reflectionVector = intersectionNormal * 2.0f * rt::DotProduct( lightVector, intersectionNormal ) - lightVector;
				specularAddition += light.color * specular * pow( rt::DotProduct( reflectionVector, rayVector.Reverse() ), 250.0f );
			}
		}
	}

	//Add contributions of ambient, diffuse and specular lighting to the final color
	rayColor += ambientAddition * rayPower;
	rayColor += diffuseAddition * rayPower;
	rayColor += specularAddition * rayPower;

	rayPower *= shininess;

	if ( reflectionDepth < reflectionLimit )
	{
		SpawnReflectionRay( intersection, rayVector, intersectionNormal, reflectionLimit, reflectionDepth, rayPower, ambientLight, lights, shapes, rayColor );
	}

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

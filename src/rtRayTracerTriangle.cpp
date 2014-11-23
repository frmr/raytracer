#include "rtRayTracer.h"
#include "rtMath.h"
#include <limits>

bool rt::RayTracer::Triangle::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& depth ) const
{
	rt::Vec3 s1 = rt::CrossProduct( rayVector, v02 );
	float divisor = rt::DotProduct( s1, v01 );

	if( divisor == 0.0f )
	{
		return false;
	}

	float invDivisor = 1.0f / divisor;
	rt::Vec3 d = rayOrigin - v0;
	float b1 = rt::DotProduct( d, s1 ) * invDivisor;

	if( b1 <= 0.0f || b1 > 1.0f )
	{
		return false;
	}

	rt::Vec3 s2 = rt::CrossProduct( d, v01 );

	float b2 = rt::DotProduct( rayVector, s2 ) * invDivisor;

	if( b2 <= 0.0f || b1 + b2 > 1.0f )
	{
		return false;
	}

	depth = rt::DotProduct( v02, s2 ) * invDivisor;

	return ( depth > 0.0f ) ? true : false;
}

bool rt::RayTracer::Triangle::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float depth, float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	rt::Vec3 intersection = rayOrigin + rayVector * depth;

	rt::Vec3 ambientAddition;
	rt::Vec3 diffuseAddition;
	rt::Vec3 specularAddition;

	for ( auto light : lights )
	{
		rt::Vec3 lightVector = ( light.origin - intersection ).Unit();
		float dotLight = rt::DotProduct( normal, lightVector );
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
				rt::Vec3 reflectionVector = normal * 2.0f * rt::DotProduct( lightVector, normal ) - lightVector;
				specularAddition += light.color * specular * pow( rt::DotProduct( reflectionVector, rayVector.Reverse() ), 250.0f );
			}
		}
	}
	rayColor += diffuseAddition; //* diffuse paramater
	rayColor += specularAddition;

	rayPower -= 1.0f - shininess;

	if ( rayPower > 0.0f )
	{
		rt::Vec3 reflectedRayVector = rayVector - normal * 2.0f * rt::DotProduct( rayVector, normal );

		float				closestDepth = std::numeric_limits<float>::max();
		shared_ptr<Shape>	closestShape = nullptr;

		//find closest intersection
		for ( auto shape : shapes )
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
			closestShape->Hit( intersection, reflectedRayVector, closestDepth, rayPower, lights, shapes, rayColor );
		}
	}

    return true;
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2 )
	:	Shape(),
		v0( v0 ), v1( v1 ), v2( v2 ),
		v01( ( v1 - v0 ) ), v02( ( v2 - v0 ) ),
        normal( rt::CrossProduct( v01.Unit(), v02.Unit() ).Unit() )
{
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 v0, const rt::Vec3 v1, const rt::Vec3 v2, const rt::Vec3 ambient, const rt::Vec3 diffuse, const rt::Vec3 specular, const float shininess )
	:	Shape( ambient, diffuse, specular, shininess ),
		v0( v0 ), v1( v1 ), v2( v2 ),
        v01( ( v1 - v0 ) ), v02( ( v2 - v0 ) ),
        normal( rt::CrossProduct( v01.Unit(), v02.Unit() ).Unit() )
{
}

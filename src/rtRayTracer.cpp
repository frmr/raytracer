#include "rtRayTracer.h"
#include "rtMath.h"
#include <cmath>

#include <iostream>
#include <limits>

rt::RayTracer::Light::Light( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
	:	origin( origin ), color( color ), intensity( intensity )
{

}

int rt::RayTracer::Shape::idCounter = 0;

rt::RayTracer::Shape::Shape( const rt::Vec3 color, const float reflectivity )
	:	id( idCounter++ ), color( color ), reflectivity( reflectivity )
{
}

bool rt::RayTracer::Triangle::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const
{
//	rt::Vec3 rayPlaneVector = v0 - rayOrigin;
//
//	rt::Vec3 s1 = Cross( ray.d, e2 );
//	float divisor = Dot(s1, e1);
//	if (divisor == 0.0f) return false;

	return false;
}

//bool rt::RayTracer::Triangle::CheckIntersection( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float rayPower, const vector<rt::RayTracer::Light>& lights, const vector<shared_ptr<rt::RayTracer::Shape>>& shapes, rt::Vec3& rayColor ) const
//{
//	//get plane intersection point
//	//check if point is within triangle
//
//}

bool rt::RayTracer::Triangle::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
    return false;
}

rt::RayTracer::Triangle::Triangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		v0( v0 ), v1( v1 ), v2( v2 ),
        v01( ( v1 - v0 ).Unit() ), v02( ( v2 - v0 ).Unit() ),
        normal( rt::CrossProduct( v01, v02 ) )
{
}

bool rt::RayTracer::Sphere::Intersects( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, float& t ) const
{
	rt::Vec3 temp = rayOrigin - origin;

	float a = rt::DotProduct( rayVector, rayVector );
	float b = 2.0f * rt::DotProduct( rayVector, temp );
	float c = rt::DotProduct( temp, temp ) - radiusSquared;
	float discriminant = ( b * b ) - ( 4.0f * a * c );

	if ( discriminant > 0.0f )
	{
		discriminant = sqrt( discriminant );
		t = ( -b - discriminant ) / ( 2.0f * a );
		return true;
	}
	else
	{
		return false;
	}
}
//
//bool rt::RayTracer::Sphere::CheckIntersection( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float rayPower, const vector<rt::RayTracer::Light>& lights, const vector<shared_ptr<rt::RayTracer::Shape>>& shapes, rt::Vec3& rayColor ) const
//{
//	float t = Intersects( rayOrigin, rayVector );
//
//	if ( t )
//	{
//		rt::Vec3 intersection = rayVector * t;
//		rt::Vec3 normal = ( intersection - origin ).Unit();
//
//		for ( auto light : lights )
//		{
//			float dotLight = rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
//			if ( dotLight > 0.0f )
//			{
//				rt::Vec3 lightVec = ( light.origin - intersection ).Unit();
//				bool occluded = false;
//				for ( const auto& shape : shapes )
//				{
//					if ( !( id == shape->id ) )
//					{
//						if ( shape->Intersects( intersection, lightVec ) > 0.0f )
//						{
//							occluded = true;
//							break;
//						}
//					}
//
//				}
//				if ( !occluded )
//				{
//					rayColor += color * rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
//				}
//			}
//
//		}
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

bool rt::RayTracer::Sphere::Hit( const rt::Vec3& rayOrigin, const rt::Vec3& rayVector, const float t, const float rayPower, const vector<Light>& lights, const vector<shared_ptr<Shape>>& shapes, rt::Vec3& rayColor ) const
{
	rt::Vec3 intersection = rayOrigin + rayVector * t;
	rt::Vec3 normal = ( intersection - origin ).Unit();

	for ( auto light : lights )
	{
		float dotLight = rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
		if ( dotLight > 0.0f )
		{
			rt::Vec3 lightVec = ( light.origin - intersection ).Unit();
			bool occluded = false;
			for ( const auto& shape : shapes )
			{
				if ( !( id == shape->id ) )
				{
					float tempT;
					if ( shape->Intersects( intersection, lightVec, tempT ) )
					{
						occluded = true;
						break;
					}
				}

			}
			if ( !occluded )
			{
				rayColor += color * rt::DotProduct( normal, ( light.origin - intersection ).Unit() );
			}
		}
	}
	return true;
}

rt::RayTracer::Sphere::Sphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
	:	Shape( color, reflectivity ),
		origin( origin ), vectorLengthSquared( origin.Length() * origin.Length() ), radius( radius ), radiusSquared( radius * radius )
{
}

bool rt::RayTracer::AddLight( const rt::Vec3 origin, const rt::Vec3 color, const float intensity )
{
	lights.push_back( Light( origin, color, intensity ) );
	return true;
}

bool rt::RayTracer::AddTriangle( const rt::Vec3 vert0, const rt::Vec3 vert1, const rt::Vec3 vert2, const rt::Vec3 color, const float reflectivity )
{
	shapes.push_back( std::make_shared<Triangle>( vert0, vert1, vert2, color, reflectivity ) );
	return true;
}

bool rt::RayTracer::AddSphere( const rt::Vec3 origin, const float radius, const rt::Vec3 color, const float reflectivity )
{
	shapes.push_back( std::make_shared<Sphere>( origin, radius, color, reflectivity ) );
	return true;
}

rt::RayTracer::rtError rt::RayTracer::Sample( const rt::Vec3& rayVector, rt::Vec3& sampleColor ) const
{
	rt::Vec3			rayOrigin;
	float				closestT = std::numeric_limits<float>::max();
	shared_ptr<Shape>	closestShape = nullptr;

	sampleColor = Vec3();

	//find closest intersection
	for ( auto shape : shapes )
	{
		float tempT;
		if ( shape->Intersects( rayOrigin, rayVector, tempT ) )
		{
			if ( tempT < closestT )
			{
				closestT = tempT;
				closestShape = shape;
			}
		}
	}

	//cast ray to closest shape
	if ( closestShape != nullptr )
	{
		closestShape->Hit( rayOrigin, rayVector, closestT, 1.0f, lights, shapes, sampleColor );
	}

	return rt::RayTracer::rtError::SUCCESS;
}

rt::RayTracer::RayTracer()
{
}

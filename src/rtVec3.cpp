#include "rtVec3.h"

#include "rtMath.h"
#include <cmath>

float rt::Vec3::Length() const
{
	return sqrt( x * x + y * y + z * z );
}

rt::Vec3 rt::Vec3::Reverse() const
{
	return rt::Vec3( -x, -y, -z );
}

rt::Vec3 rt::Vec3::Unit() const
{
	float length = sqrt( x * x + y * y + z * z );
	return rt::Vec3( x / length, y / length, z / length );
}

rt::Vec3 rt::Vec3::UnitCap() const
{
	return rt::Vec3( ( x > 1.0f ) ? 1.0f : x, ( y > 1.0f ) ? 1.0f : y, ( z > 1.0f ) ? 1.0f : z );
}

rt::Vec3 rt::Vec3::operator+( const Vec3& rhs ) const
{
	return rt::Vec3( x + rhs.x, y + rhs.y, z + rhs.z );
}

rt::Vec3 rt::Vec3::operator-( const Vec3& rhs ) const
{
	return rt::Vec3( x - rhs.x, y - rhs.y, z - rhs.z );
}

rt::Vec3 rt::Vec3::operator*( const float& rhs ) const
{
	return rt::Vec3( x * rhs, y * rhs, z * rhs );
}

rt::Vec3 rt::Vec3::operator*( const Vec3& rhs ) const
{
	return rt::Vec3( x * rhs.x, y * rhs.y, z * rhs.z );
}

rt::Vec3 rt::Vec3::operator/( const float& rhs ) const
{
	return rt::Vec3( x / rhs, y / rhs, z / rhs );
}

rt::Vec3& rt::Vec3::operator+=( const Vec3& rhs )
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

rt::Vec3& rt::Vec3::operator-=( const Vec3& rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

rt::Vec3& rt::Vec3::operator*=( const float& rhs )
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

rt::Vec3& rt::Vec3::operator*=( const rt::Vec3& rhs )
{
	x *= rhs.x;
	y *= rhs.y;
	z *= rhs.z;
	return *this;
}

rt::Vec3& rt::Vec3::operator/=( const float& rhs )
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

rt::Vec3::Vec3()
	:	x( 0.0f ), y( 0.0f ), z( 0.0f )
{
}

rt::Vec3::Vec3( const float x, const float y, const float z )
	:	x( x ), y( y ), z( z )
{
}

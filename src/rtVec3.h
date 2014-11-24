#ifndef RT_VEC3_H
#define RT_VEC3_H

namespace rt
{
	class GeoCoord;

	class Vec3
	{
	public:
		float	x, y, z;

	public:
		float			Length() const;
		Vec3			Reverse() const;
		Vec3			Round() const;
		Vec3			Unit() const;
		Vec3			UnitCap() const;

		Vec3			operator+( const Vec3& rhs ) const;
		Vec3			operator-( const Vec3& rhs ) const;
		Vec3			operator*( const float& rhs ) const;
		Vec3			operator*( const Vec3& rhs ) const;
		Vec3			operator/( const float& rhs ) const;

		Vec3&			operator+=( const Vec3& rhs );
		Vec3&			operator-=( const Vec3& rhs );
		Vec3&			operator*=( const float& rhs );
		Vec3&			operator*=( const Vec3& rhs );
		Vec3&			operator/=( const float& rhs );

	public:
		Vec3();
		Vec3( const float x, const float y, const float z );
	};
}
#endif // RT_VEC3_H

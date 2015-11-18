#pragma once

#include "Common.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Collection of mathematical functions.
/////////////////////////////////////////////////////////////////////////////////////////
namespace Math
{
	inline float DistanceSquared( const Vector2D& a, const Vector2D& b )
	{
		float xDiff = a.x - b.x;
		float yDiff = a.y - b.y;
		return xDiff * xDiff + yDiff * yDiff;
	}

	inline float Distance( const Vector2D& a, const Vector2D& b )
	{
		return sqrt( DistanceSquared( a, b ) );
	}

	inline float Distance( const Vector2D& v )
	{
		return sqrt( v.x * v.x + v.y * v.y );
	}

	inline Vector2D Normalize( const Vector2D& vector )
	{
		float length = Distance( vector );

		Vector2D v;
		if (length != 0)
		{
			v.x = vector.x / length;
			v.y = vector.y / length;
		}

		return v;
	}

	inline bool IsApproximatelyEqual( float a, float b )
	{
		return abs( a - b ) < FLT_EPSILON;
	}

	inline float Clamp( float value, float min, float max )
	{
		return std::max( min, std::min( value, max ) );
	}
}
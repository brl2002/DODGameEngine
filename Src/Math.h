#pragma once

#include "Common.h"

namespace Math
{
	Vector2D& Normalize(const Vector2D& vector)
	{
		float normTotal = vector.x + vector.y;
		return Vector2D(vector.x / normTotal, vector.y / normTotal);
	}

	inline bool IsEqual(float a, float b)
	{
		return abs(a - b) < FLT_EPSILON;
	}
}
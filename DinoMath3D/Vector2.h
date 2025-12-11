#include <string>
#pragma once

namespace DinoMath3D
{
	struct vec2
	{
		vec2(float X, float Y) : x(X), y(Y) {}
		vec2(float s) : x(s), y(s) {}
		vec2(float* v) : x(v[0]), y(v[1]) {}
		vec2(const vec2& rhs) = default;
		vec2() = default;

		union
		{
			struct
			{
				float x, y;
			};
			float data[2];
		};
		
		vec2 operator + (const vec2& rhs) const;
		vec2 operator - (const vec2& rhs) const;
		vec2 operator * (const vec2& rhs) const;
		vec2 operator / (const vec2& rhs) const;

		vec2 operator + (const float s) const;
		vec2 operator - (const float s) const;
		vec2 operator * (const float s) const;
		vec2 operator / (const float s) const;

		vec2 operator += (const vec2& rhs) { *this = *this + rhs; return *this; }
		vec2 operator -= (const vec2& rhs) { *this = *this - rhs; return *this; }
		vec2 operator *= (const vec2& rhs) { *this = *this * rhs; return *this; }
		vec2 operator /= (const vec2& rhs) { *this = *this / rhs; return *this; }

		operator std::string() { return "[" + std::to_string(x) + ", " + std::to_string(y) + "]"; }
	};
}
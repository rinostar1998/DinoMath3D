#pragma once
#include "SimdFunctions.h"
#include <string>
#include <immintrin.h>

namespace DinoMath3D
{
	struct vec3
	{
		inline vec3(float X, float Y, float Z) : reg(_mm_set_ps(0.0f, Z, Y, X)) {}
		inline vec3(float s) : reg(_mm_set1_ps(s)) {}
		inline vec3(float* v) : reg(_mm_set_ps(0.0f, v[0], v[1], v[2])) {}
		inline vec3() : reg(_mm_set1_ps(0.0f)) {}
		inline vec3(const vec3& rhs) = default;
		inline vec3(__m128 m) : reg(m) {}

		union
		{
			struct
			{
				float x, y, z;
			};
			__m128 reg;
			float data[3];
		};

		inline void* operator new[](size_t x) { return simd_malloc(x); }
		inline void operator delete[](void* x) { if (x) simd_free(x); }

		vec3 operator + (const vec3& rhs) const;
		vec3 operator - (const vec3& rhs) const;
		vec3 operator * (const vec3& rhs) const;
		vec3 operator / (const vec3& rhs) const;

		vec3 operator + (const float rhs) const;
		vec3 operator - (const float rhs) const;
		vec3 operator * (const float rhs) const;
		vec3 operator / (const float rhs) const;
		
		vec3 operator += (const vec3& rhs) { *this = *this + rhs; return *this; }
		vec3 operator -= (const vec3& rhs) { *this = *this - rhs; return *this; }
		vec3 operator *= (const vec3& rhs) { *this = *this * rhs; return *this; }
		vec3 operator /= (const vec3& rhs) { *this = *this / rhs; return *this; }
		float len(const vec3& lhs);

		static vec3 cross(const vec3& lhs, const vec3& rhs);
		static float dot(const vec3& lhs, const vec3& rhs);

		operator float* () { return data; }

		operator std::string() { return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "]"; }
	};
}
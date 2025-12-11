#pragma once
#include "SimdFunctions.h"
#include "Vector3.h"
#include <string>
#include <immintrin.h>

namespace DinoMath3D
{
	class vec4
	{
	public:
		inline vec4(float X, float Y, float Z, float W) : reg(_mm_set_ps(W, Z, Y, X)) {}
		inline vec4(float s) : reg(_mm_set1_ps(s)) {}
		inline vec4(float* v) : reg(_mm_set_ps(v[3], v[2], v[1], v[0])) {}
		inline vec4() : reg(_mm_set1_ps(0.0f)) {}
		inline vec4(vec3 v) : reg(v.reg) {}
		inline vec4(const vec4& rhs) = default;
		inline vec4(__m128 m) : reg(m) {}

		union
		{
			struct
			{
				float x, y, z, w;
			};
			__m128 reg;
			float data[4];
		};

		inline void* operator new[](size_t x) { return simd_malloc(x); }
		inline void operator delete[](void* x) { if (x) simd_free(x); }

		vec4 operator + (const vec4& rhs) const;
		vec4 operator - (const vec4& rhs) const;
		vec4 operator * (const vec4& rhs) const;
		vec4 operator / (const vec4& rhs) const;

		vec4 operator + (const float rhs) const;
		vec4 operator - (const float rhs) const;
		vec4 operator * (const float rhs) const;
		vec4 operator / (const float rhs) const;

		vec4 operator += (const vec4& rhs) { *this = *this + rhs; return *this; }
		vec4 operator -= (const vec4& rhs) { *this = *this - rhs; return *this; }
		vec4 operator *= (const vec4& rhs) { *this = *this * rhs; return *this; }
		vec4 operator /= (const vec4& rhs) { *this = *this / rhs; return *this; }

		float len();

		static float dot(const vec4& lhs, const vec4& rhs);
		vec4 normalize();

		operator float* () { return data; }

		operator std::string() { return "[" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + "]"; }
	};
}
#pragma once
#include "Vector4.h"
#include "Vector3.h"

namespace DinoMath3D
{
	class quat
	{
	public:
		inline quat(float X, float Y, float Z, float W) : reg(_mm_set_ps(W, Z, Y, X)) {}
		inline quat(float s) : reg(_mm_set1_ps(s)) {}
		inline quat(float* v) : reg(_mm_set_ps(v[3], v[2], v[1], v[0])) {}
		inline quat() : reg(_mm_set1_ps(0.0f)) {}
		inline quat(const quat& rhs) = default;
		inline quat(__m128 m) : reg(m) {}

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

		quat operator + (const quat& rhs) const;
		quat operator - (const quat& rhs) const;
		quat operator * (const quat& rhs) const;
		quat operator / (const quat& rhs) const noexcept { throw(false, "nooyuu, fuck you!"); return quat(0.0f); }

		vec4 operator + (const float rhs) const;
		vec4 operator - (const float rhs) const;
		quat operator * (const float rhs) const;
		quat operator / (const float rhs) const;

		static float dot(const quat& lhs, const quat& rhs);
		float len();
		quat normalize();

		operator std::string() { return "(" + std::to_string(x) + "i, " + std::to_string(y) + "j, " + std::to_string(z) + "k, " + std::to_string(w) + ")"; }

	};
}
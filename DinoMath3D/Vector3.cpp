#include "Vector3.h"
#include <cmath>
#include <memory>

void* simd_malloc(const size_t size) {
#if defined PLATFORM_WINDOWS
	return _aligned_malloc(size, 16);
#elif defined PLATFORM_LINUX
	return memalign(16, size);
#elif defined PLATFORM_MACOS
	return malloc(size);
#else // use page-aligned memory for other systems
	return valloc(size);
#endif
}

void simd_free(void* v) {
#if defined PLATFORM_WINDOWS
	return _aligned_free(v);
#else
	return free(v);
#endif
}

namespace DinoMath3D
{
	vec3 vec3::operator + (const vec3& rhs) const
	{
		return _mm_add_ps(this->reg, rhs.reg);
	}

	vec3 vec3::operator - (const vec3& rhs) const
	{
		return _mm_sub_ps(this->reg, rhs.reg);
	}

	vec3 vec3::operator * (const vec3& rhs) const
	{
		return _mm_mul_ps(this->reg, rhs.reg);
	}

	vec3 vec3::operator / (const vec3& rhs) const
	{
		return _mm_div_ps(this->reg, rhs.reg);
	}

	vec3 vec3::operator + (const float rhs) const
	{
		return _mm_add_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec3 vec3::operator - (const float rhs) const
	{
		return _mm_sub_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec3 vec3::operator * (const float rhs) const
	{
		return _mm_mul_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec3 vec3::operator / (const float rhs) const
	{
		return _mm_div_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec3 vec3::cross(const vec3& lhs, const vec3& rhs)
	{
		__m128 _a = _mm_shuffle_ps(lhs.reg, lhs.reg, _MM_SHUFFLE(3, 0, 2, 1));
		__m128 _b = _mm_shuffle_ps(rhs.reg, rhs.reg, _MM_SHUFFLE(3, 1, 0, 2));
		__m128 _c = _mm_mul_ps(_a, rhs.reg);
		__m128 _d = _mm_shuffle_ps(_c, _c, _MM_SHUFFLE(3, 0, 2, 1));
		return _mm_fmsub_ps(_a, _b, _d);
	}

	float vec3::dot(const vec3& lhs, const vec3& rhs)
	{
		__m128 _a = _mm_mul_ps(lhs.reg, rhs.reg);
		__m128 _b = _mm_hadd_ps(_a, _a);
		_b = _mm_hadd_ps(_b, _b);
		return std::_Bit_cast<float>(_mm_extract_ps(_b, 0));
	}

	float len(const vec3& lhs)
	{
		__m128 _a = _mm_mul_ps(lhs.reg, lhs.reg);
		__m128 _b = _mm_hadd_ps(_a, _a);
		_b = _mm_hadd_ps(_b, _b);
		return std::sqrt(std::_Bit_cast<float>(_mm_extract_ps(_b, 0)));
	}
}
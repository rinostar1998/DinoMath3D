#include "Vector4.h"
#include <memory>
#include <cmath>

namespace DinoMath3D
{
	vec4 vec4::operator + (const vec4& rhs) const
	{
		return _mm_add_ps(this->reg, rhs.reg);
	}

	vec4 vec4::operator - (const vec4& rhs) const
	{
		return _mm_sub_ps(this->reg, rhs.reg);
	}

	vec4 vec4::operator * (const vec4& rhs) const
	{
		return _mm_mul_ps(this->reg, rhs.reg);
	}

	vec4 vec4::operator / (const vec4& rhs) const
	{
		return _mm_div_ps(this->reg, rhs.reg);
	}

	vec4 vec4::operator + (const float rhs) const
	{
		return _mm_add_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec4 vec4::operator - (const float rhs) const
	{
		return _mm_sub_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec4 vec4::operator * (const float rhs) const
	{
		return _mm_mul_ps(this->reg, _mm_set_ps1(rhs));
	}

	vec4 vec4::operator / (const float rhs) const
	{
		return _mm_div_ps(this->reg, _mm_set_ps1(rhs));
	}

	float vec4::dot(const vec4& lhs, const vec4& rhs)
	{
		__m128 _a = _mm_mul_ps(lhs.reg, rhs.reg);
		__m128 _b = _mm_hadd_ps(_a, _a);
		_b = _mm_hadd_ps(_b, _b);
		return std::_Bit_cast<float>(_mm_extract_ps(_b, 0));
	}

	float vec4::len()
	{
		__m128 _a = _mm_mul_ps(this->reg, this->reg);
		__m128 _s = _mm_hadd_ps(_a, _a);
		_s = _mm_hadd_ps(_s, _s);
		return std::sqrt(std::_Bit_cast<float>(_mm_extract_ps(_s, 0)));
	}

	vec4 vec4::normalize()
	{
		return *this / this->len();
	}
}
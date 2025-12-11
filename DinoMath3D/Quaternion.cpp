#include "Quaternion.h"
#include <cmath>

namespace DinoMath3D
{
	quat quat::operator + (const quat& rhs) const
	{
		return _mm_add_ps(this->reg, rhs.reg);
	}

	quat quat::operator - (const quat& rhs) const
	{
		return _mm_sub_ps(this->reg, rhs.reg);
	}

	quat quat::operator * (const quat& rhs) const
	{
		__m128 _a = _mm_shuffle_ps(this->reg, this->reg, _MM_SHUFFLE(0, 1, 2, 3));
		__m128 _b = _mm_shuffle_ps(rhs.reg, rhs.reg, _MM_SHUFFLE(0, 1, 0, 1));
		__m128 _c = _mm_shuffle_ps(rhs.reg, rhs.reg, _MM_SHUFFLE(2, 3, 2, 3));
		__m128 _d = _mm_hsub_ps(_mm_mul_ps(this->reg, _b), _mm_mul_ps(_a, _c));
		__m128 _e = _mm_hadd_ps(_mm_mul_ps(this->reg, _c), _mm_mul_ps(_a, _b));
		__m128 _f = _mm_addsub_ps(_mm_shuffle_ps(_e, _d, _MM_SHUFFLE(3, 2, 1, 0)),
			_mm_shuffle_ps(_d, _e, _MM_SHUFFLE(2, 3, 0, 1)));
		return _mm_shuffle_ps(_f, _f, _MM_SHUFFLE(2, 1, 3, 0));
	}

	quat quat::operator * (const float rhs) const
	{
		return _mm_mul_ps(this->reg, _mm_set_ps1(rhs));
	}

	quat quat::operator / (const float rhs) const
	{
		return _mm_div_ps(this->reg, _mm_set_ps1(rhs));
	}

	float quat::len()
	{
		__m128 _a = _mm_mul_ps(this->reg, this->reg);
		__m128 _s = _mm_hadd_ps(_a, _a);
		_s = _mm_hadd_ps(_s, _s);
		return std::sqrt(std::_Bit_cast<float>(_mm_extract_ps(_s, 0)));
	}

	quat quat::normalize()
	{
		return *this / this->len();
	}

	float quat::dot(const quat& lhs, const quat& rhs)
	{
		__m128 _a = _mm_mul_ps(lhs.reg, rhs.reg);
		__m128 _b = _mm_hadd_ps(_a, _a);
		_b = _mm_hadd_ps(_b, _b);
		return std::_Bit_cast<float>(_mm_extract_ps(_b, 0));
	}
}
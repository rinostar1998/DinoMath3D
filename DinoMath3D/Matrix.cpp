#include "Matrix.h"

namespace DinoMath3D
{
	Matrix<4, 4>::Matrix(const float s) noexcept
	{ // I hate doing col major, but thats "standard" for opengl and glm!!!
		m_register128[0] = _mm_set_ps(s, 0.0f, 0.0f, 0.0f);
		m_register128[1] = _mm_set_ps(0.0f, s, 0.0f, 0.0f);
		m_register128[2] = _mm_set_ps(0.0f, 0.0f, s, 0.0f);
		m_register128[3] = _mm_set_ps(0.0f, 0.0f, 0.0f, s);
	}
	
	Matrix<4, 4>::Matrix(const Matrix<4, 4>& rhs) noexcept
	{
		for(uint8_t i = 0; i < 4; i++)
			m_register128[i] = rhs.m_register128[i];
	}

	Matrix<4, 4>::Matrix(const __m128* reg) noexcept
	{
		for (uint8_t i = 0; i < 4; i++)
			m_register128[i] = reg[i];
	}
	
	Matrix<4, 4>::Matrix(const __m256* reg)
	{
		m_register256[0] = reg[0];
		m_register256[1] = reg[1];
	}
	
	Matrix<4, 4> Matrix<4, 4>::operator + (const Matrix<4, 4>& rhs)
	{
		Matrix<4, 4> ret = Matrix<4, 4>(0.0f);
#if USE_512
		ret.m_register512 = _mm512_add_ps(m_register512, rhs.m_register512);
#else
		for (uint8_t i = 0; i < 4; i++)
		{
			ret.m_register128[i] = _mm_add_ps(m_register128[i], rhs.m_register128[i]);
		}
#endif
		return ret;
	}

	Matrix<4, 4> Matrix<4, 4>::operator - (const Matrix<4, 4>& rhs)
	{
		Matrix<4, 4> ret = Matrix<4, 4>(0.0f);
#if USE_512
		ret.m_register512 = _mm512_sub_ps(m_register512, rhs.m_register512);
#else
		for (uint8_t i = 0; i < 4; i++)
		{
			ret.m_register128[i] = _mm_sub_ps(m_register128[i], rhs.m_register128[i]);
		}
#endif
		return ret;
	}

	Matrix<4, 4> __vectorcall Matrix<4, 4>::operator * (const Matrix<4, 4>& rhs)
	{
		__m128 _a = rhs.m_register128[3];
		__m128 _b = rhs.m_register128[2];
		__m128 _c = rhs.m_register128[1];
		__m128 _d = rhs.m_register128[0];
		Matrix<4, 4> ret;
		for (int i = 0; i < 4; i++) {
			__m128 brod1 = _mm_set1_ps(m_data[4 * i + 0]);
			__m128 brod2 = _mm_set1_ps(m_data[4 * i + 1]);
			__m128 brod3 = _mm_set1_ps(m_data[4 * i + 2]);
			__m128 brod4 = _mm_set1_ps(m_data[4 * i + 3]);
			__m128 row = _mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(brod1, _a),
					_mm_mul_ps(brod2, _b)),
				_mm_add_ps(
					_mm_mul_ps(brod3, _c),
					_mm_mul_ps(brod4, _d)));
			_mm_store_ps(&ret.m_data[4 * i], row);
		}
		return ret;
	}
	 
	vec4 Matrix<4, 4>::operator * (const vec4& rhs)
	{
		vec4 ret = vec4(0.0f);
		for (uint8_t i = 0; i < 4; i++)
		{
			__m128 _b = _mm_set_ps1(rhs.data[i]);
			ret.data[i] = std::_Bit_cast<float>(_mm_extract_ps(_mm_mul_ps(m_register128[i], _b), 0));
		}
		return ret;
	}

	void Matrix<4, 4>::Scale(const float s)
	{
		mat4 ret;
		__m128i _i = _mm_set_epi32(0, 5, 10, 0); // indices for scaled identity matrix
		__m128 _a = _mm_i32gather_ps(this->m_data, _i, 4);
		__m128 _b = _mm_set1_ps(s);
		_a = _mm_mul_ps(_a, _b);
		this->m_data[0] = _a.m128_f32[0];
		this->m_data[5] = _a.m128_f32[1];
		this->m_data[10] = _a.m128_f32[2];
	}

	void Matrix<4, 4>::Scale(const vec3& s)
	{
		__m128i _i = _mm_set_epi32(0, 5, 10, 0); // indices for scaled identity matrix
		__m128 _a = _mm_i32gather_ps(this->m_data, _i, 4);
		_a = _mm_mul_ps(_a, s.reg);
		this->m_data[0] = _a.m128_f32[0];
		this->m_data[5] = _a.m128_f32[1];
		this->m_data[10] = _a.m128_f32[2];
	}
}

bool DinoMath3D::Matrix<4, 4>::operator==(const Matrix& other) const
{
	for (int i = 0; i < 4; i++)
	{
		if (_mm_cmpneq_ps(this->m_register128[i], other.m_register128[i]).m128_f32[0] == 0xffffffff)
		{
			return false;
		}
	}
	return true;
}

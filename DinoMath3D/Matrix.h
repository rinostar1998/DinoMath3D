#pragma once
#include <immintrin.h>
#include "Vector4.h"
#include "Vector3.h"

#define USE_512 0

namespace DinoMath3D // Matrix will be COLUMN major for glm reasons.
{
	template<int col, int row>
	class Matrix
	{
	};

	template<>
	class Matrix<4, 4>
	{
	public:

		Matrix(const float s) noexcept;
		Matrix(const Matrix<4, 4>& mat4) noexcept;
		Matrix(const __m128* reg) noexcept;
		Matrix(const __m256* reg);
		Matrix()
		{
			for (int i = 0; i < 4; i++)
			{
				m_register128[i] = _mm_set_ps1(0.0f);
			} 
		}

		Matrix<4, 4> __vectorcall operator + (const Matrix<4, 4>& rhs);
		Matrix<4, 4> __vectorcall operator - (const Matrix<4, 4>& rhs);
		Matrix<4, 4> __vectorcall operator * (const Matrix<4, 4>& rhs);
		bool operator==(const Matrix& other) const;
		vec4 __vectorcall operator * (const vec4& rhs);

		void Scale(const float s);
		void Scale(const vec3& s);

		operator std::string()
		{
			std::string debug = "\n";
			for (int8_t i = 3; i > -1; --i)
			{
				debug += "[";
				for (uint8_t j = 0; j < 4; j++)
				{
					debug += std::to_string(m_data[i + (j * 4)]) + " ";
				}
				debug += "]\n";
			}
			return debug;
		}

	private:
		union
		{
			float m_data[16];
			float m_matrix[4][4];
			__m128 m_register128[4];
			__m256 m_register256[2];
			__m512 m_register512;
			vec4 vec[4];
		};
	
	friend class Math;
	};

	typedef Matrix<4, 4> mat4;
}
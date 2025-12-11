#include "Math.h"
#include <string>
#include <cmath>

namespace DinoMath3D
{
	mat4 Math::CreateRotationMatrix(const quat& quat)
	{
        __m128 _1, _2, _a, _b, _c, _d;
        __m128 _x, _y, _z, _mul_mask;
        __m128i _i;

        float _data[12];

        vec3 a = vec3((2 * quat.x * quat.y), (2 * quat.x * quat.z), (2 * quat.y * quat.z));
        vec3 b = vec3((2 * quat.w * quat.x), (2 * quat.w * quat.y), (2 * quat.w * quat.z));
        vec3 c = vec3((2 * quat.x * quat.x), (2 * quat.y * quat.y), (2 * quat.z * quat.z));

        vec4 x = vec3(1 - c.data[1] - c.data[2], 1 - c.data[0] - c.data[2], 1 - c.data[0] - c.data[1]);
        vec4 y = vec3(a.data[0] + b.data[2], a.data[2] + b.data[0], a.data[1] + b.data[1]);
        vec4 z = vec3(a.data[1] - b.data[1], a.data[0] - b.data[2], a.data[2] - b.data[0]);

        mat4 ret;
        //// init constants
        _1 = _mm_set1_ps(1.0f);
        _2 = _mm_set1_ps(2.0f);
        _mul_mask = _mm_set_ps(0.0f, 1.0f, 1.0f, 1.0f);
        // chunk a : [ 2xy, 2xz, 2yz, ? (2xx) to be ignored ]
        _a = _mm_shuffle_ps(quat.reg, quat.reg, _MM_SHUFFLE(0, 1, 0, 0)); // reversed, and vec3 are smaller then vec4, so 1 extra garbage elem.
        _b = _mm_shuffle_ps(quat.reg, quat.reg, _MM_SHUFFLE(0, 2, 2, 1));
        _c = _mm_mul_ps(_a, _b);
        _a = _mm_mul_ps(_c, _2); // A = [ 2xy, 2xz, 2yz, ? ]
        // chunk b : [ 2wx, 2wy, 2wz, 2ww ]
        _b = _mm_set1_ps(quat.w);
        _c = _mm_mul_ps(_b, quat.reg);
        _b = _mm_mul_ps(_2, _c);
        // chunk c : [ 2x^2, 2y^2, 2z^2 ]
        _c = _mm_mul_ps(quat.reg, quat.reg);
        _c = _mm_mul_ps(_2, _c);
        // Now for the intricate load instructions:
        // The chunks come together to from each entry, which is similar enough to be vectors!
        // X = [ 1 - (C[1] + C[2]), 1 - (C[0] + C[2]), 1 - (C[0] + C[1]) ]
        _d = _mm_shuffle_ps(_c, _c, _MM_SHUFFLE(0, 0, 0, 1));
        _x = _mm_shuffle_ps(_c, _c, _MM_SHUFFLE(0, 1, 2, 2));
        _x = _mm_add_ps(_d, _x);
        _x = _mm_sub_ps(_1, _x);
        _x = _mm_mul_ps(_x, _mul_mask);
        _mm_store_ps(_data, _x);
        // Y = [A[0] + B[2], A[2] + B[0], A[1] + B[1]]
        _d = _mm_shuffle_ps(_a, _a, _MM_SHUFFLE(0, 0, 2, 1));
        _y = _mm_shuffle_ps(_b, _b, _MM_SHUFFLE(0, 2, 0, 1));
        _y = _mm_add_ps(_y, _d);
        _y = _mm_mul_ps(_y, _mul_mask);
        _mm_store_ps(&_data[4], _y);
        // Z = [A[1] - B[1], A[0] - B[2], A[2] - B[0]]
        _d = _mm_shuffle_ps(_a, _a, _MM_SHUFFLE(0, 2, 0, 1));
        _c = _mm_shuffle_ps(_b, _b, _MM_SHUFFLE(0, 0, 2, 1));
        _z = _mm_sub_ps(_d, _c);
        _z = _mm_mul_ps(_z, _mul_mask);
        _mm_store_ps(&_data[8], _z);
        //In column major, top elements in column [i] are assumed to be elem [i][0].
        //a, b, c, d are now columns.
        //a = [X[0], Y[0], Z[0], 0]
        //b = [Z[1], X[1], Y[1], 0]
        //c = [Y[2], Z[2], X[2], 0]
        //d = [0, 0, 0, 1]

        _i = _mm_set_epi32(0, 4, 8, 11);
        ret.m_register128[0] = _mm_i32gather_ps((float*)&_data, _i, 4);

        _i = _mm_set_epi32(9, 1, 5, 11);
        ret.m_register128[1] = _mm_i32gather_ps((float*)&_data, _i, 4);

        _i = _mm_set_epi32(6, 10, 2, 11);
        ret.m_register128[2] = _mm_i32gather_ps((float*)&_data, _i, 4);

        ret.m_register128[3] = _mm_set_ps(0, 0, 0, 1);
        return ret;
	}

    mat4 Math::Rotate(mat4& mat, const quat& quat)
    {
        return CreateRotationMatrix(quat) * mat;
    }

    mat4 Math::MakeOrthographic(
        const float left, const float right,
        const float bottom, const float top,
        const float near, const float far)
    {
        mat4 ret;
        __m128 _mask = _mm_set_ps(1.0f, -2.0f, 2.0f, 2.0f);
        __m128 _a = _mm_set_ps(bottom, top, left, right);
        __m128 _b = _mm_set_ps(0.0f, 0.0f, near, far);
        __m128 _c = _mm_hsub_ps(_a, _b); // a = [ r - l, t - b, f - n, 0 ]
        __m128 _d = _mm_hadd_ps(_a, _b); // a = [ r + l, t + b, f + n, 0 ]
        _d = _mm_mul_ps(_mm_div_ps(_d, _c), _mm_set_ps1(-1.0f));
        _c = _mm_div_ps(_mask, _c);
        ret.m_matrix[0][3] = _c.m128_f32[0];
        ret.m_matrix[0][0] = _d.m128_f32[0];
        ret.m_matrix[1][2] = _c.m128_f32[1];
        ret.m_matrix[1][0] = _d.m128_f32[1];
        ret.m_matrix[2][1] = _c.m128_f32[2];
        ret.m_matrix[2][0] = _d.m128_f32[2];
        ret.m_matrix[3][0] = 1.0f;
        return ret;
    }

    mat4 Math::MakePerspective(
        const float fov, const float aspectRatio,
        const float near, const float far)
    {
        mat4 ret;
        float view = std::tan(fov / 2);
        float z_delta = far - near;
        __m128 _a = _mm_set_ps(-2 * far * near, -(far + near), 1.0f, 1.0f);
        __m128 _b = _mm_set_ps(far - near, far - near, view, aspectRatio * view);
        _a = _mm_div_ps(_a, _b);
        ret.m_matrix[3][1] = -1.0f;
        ret.m_matrix[0][3] = _a.m128_f32[0];
        ret.m_matrix[1][2] = _a.m128_f32[1];
        ret.m_matrix[2][1] = _a.m128_f32[2];
        ret.m_matrix[2][0] = _a.m128_f32[3];
        return ret;
    }
}
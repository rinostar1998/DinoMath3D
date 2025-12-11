#pragma once

//#include "DinoMath3D/Math/Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix.h"

// goddamn it windows!
#undef near
#undef far

namespace DinoMath3D
{
	class Math
	{
	public:
		static mat4 CreateRotationMatrix(const quat& quat);
		static mat4 Rotate(mat4& mat, const quat& quat);
		static mat4 MakeOrthographic(
			const float left, const float right,
			const float bottom, const float top,
			const float near, const float far);
		static mat4 MakePerspective(
			const float fov, const float aspectRatio,
			const float near, const float far);
	};
}
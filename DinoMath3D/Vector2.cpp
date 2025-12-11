#include "Vector2.h"

namespace DinoMath3D
{
	vec2 vec2::operator + (const vec2& rhs) const
	{
		return vec2(this->x + rhs.x, this->y + rhs.y);
	}

	vec2 vec2::operator - (const vec2& rhs) const
	{
		return vec2(this->x - rhs.x, this->y - rhs.y);
	}

	vec2 vec2::operator * (const vec2& rhs) const
	{
		return vec2(this->x * rhs.x, this->y * rhs.y);
	}

	vec2 vec2::operator / (const vec2& rhs) const
	{
		return vec2(this->x / rhs.x, this->y / rhs.y);
	}

	vec2 vec2::operator + (const float rhs) const
	{
		return vec2(this->x + rhs, this->y + rhs);
	}

	vec2 vec2::operator - (const float rhs) const
	{
		return vec2(this->x - rhs, this->y - rhs);
	}

	vec2 vec2::operator * (const float rhs) const
	{
		return vec2(this->x * rhs, this->y * rhs);
	}

	vec2 vec2::operator / (const float rhs) const
	{
		return vec2(this->x / rhs, this->y / rhs);
	}
}
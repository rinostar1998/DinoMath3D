#include <immintrin.h>
#include <stdio.h>

namespace DinoMath3D
{
	void* simd_malloc(const size_t size);
	void simd_free(void* v);

	float hsum(__m128 x);
	float hsum(__m256 x);
	int hsum(__m256i x);
}
#include "SimdFunctions.h"
#include <memory>

namespace DinoMath3D
{
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

	float hsum(__m128 x)
	{
		__m128 shuf = _mm_movehdup_ps(x);
		__m128 sums = _mm_add_ps(x, shuf);
		shuf = _mm_movehl_ps(shuf, sums);
		sums = _mm_add_ss(sums, shuf);
		return _mm_cvtss_f32(sums);
	}

	float hsum(__m256 x)
	{
		__m128 l = _mm256_extractf128_ps(x, 0);
		__m128 h = _mm256_extractf128_ps(x, 1);
		l = _mm_add_ps(l, h);
		l = _mm_hadd_ps(l, l);
		return std::_Bit_cast<float>(_mm_extract_ps(l, 0)) + std::_Bit_cast<float>(_mm_extract_ps(l, 1));
	}

	int hsum(__m256i x) {
		__m128i l = _mm256_extracti128_si256(x, 0);
		__m128i h = _mm256_extracti128_si256(x, 1);
		l = _mm_add_epi32(l, h);
		l = _mm_hadd_epi32(l, l);
		return _mm_extract_epi32(l, 0) + _mm_extract_epi32(l, 1);
	}
}
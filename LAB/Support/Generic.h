#pragma once
#include <concepts>
#include <cstdint>
#include <bit>

//#define USING_CMATH

#include "../Debugging.h"


#if defined(LAB_USING_SSE) || defined(LAB_USING_AVX2)
#define USING_SIMD
#include <immintrin.h>
#endif

namespace lab {


	template<std::floating_point F>
	LAB_constexpr F Abs(F const input) {
		return input * (F(1) - F(2) * (input < F(0)));
	}


	template<std::floating_point F>
	LAB_constexpr F Trunc(F const input) {
		//stealing from ccmath a little bit
		//https://github.com/Rinzii/ccmath
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check(input);
#endif
		
		if constexpr(std::is_same_v<F, float>) {
			const uint32_t bits = std::bit_cast<uint32_t>(input);

			const int exponent = ((bits >> 23) & 0xFF) - 127;

			if (exponent >= 23) {
				return input;  // Already an integer
			}
			if (exponent < 0) {
				return 0.f;
			}

			// Perform the truncation
			const int trimming_size = 23 - (exponent);

			LAB_constexpr uint32_t mantissa_mask = (1 << 23) - 1;
			const auto truncated_mantissa = static_cast<uint32_t>(((bits & mantissa_mask) >> trimming_size) << trimming_size);

			LAB_constexpr uint32_t mantissa_anti_mask = UINT32_MAX ^ mantissa_mask;
			return std::bit_cast<float>((bits & mantissa_anti_mask) | truncated_mantissa);
		}
		else if constexpr (std::is_same_v<F, double>) {
			const uint64_t bits = std::bit_cast<uint64_t>(input);
			const int64_t exponent = ((bits >> 52) & 0b11111111111); //11 bits
			if(exponent >= 52){
				return input;
			}
			if(exponent < 0){
				return 0.0;
			}
			const int64_t trimming_size = 52 - (exponent);

			LAB_constexpr uint64_t mantissa_mask = ((uint64_t)1 << (uint64_t)52) - (uint64_t)1;
			const auto truncated_mantissa = static_cast<uint64_t>(((bits & mantissa_mask) >> trimming_size) << trimming_size);

			LAB_constexpr uint64_t mantissa_anti_mask = UINT64_MAX ^ mantissa_mask;
			return std::bit_cast<double>((bits & mantissa_anti_mask) | truncated_mantissa);
		}
	}
	template<std::floating_point F>
	LAB_constexpr F Mod(F const x, F const y) {
		//stealing from ccmath a little bit
		//https://github.com/Rinzii/ccmath
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(x);
		Debug_Anomaly_Check(y);
#endif
		
		//https://quick-bench.com/q/SzfiLPh2admpveSGNjQrUV5D6Qg
		return x - (Trunc(x / y) * y);
	}
	

	template<typename T>
	requires(std::is_fundamental_v<T>)
	LAB_constexpr T Max(T const a, T const b){
		const bool aBigger = a > b;
		return a * aBigger + b * !aBigger;
	}
	template<typename T>
		requires(std::is_fundamental_v<T>)
	LAB_constexpr T Min(T const a, T const b){
		const bool aSmaller = a < b;
		return a * aSmaller + b * !aSmaller;
	}
	template<typename T>
		requires(std::is_fundamental_v<T>)
	LAB_constexpr T Clamp(T const val, T const min, T const max) {
		return Min(Max(val, min), max);
	}

	template<std::floating_point F>
	LAB_constexpr F Mix(F const lhs, F const rhs, F const weight){
		if (weight < F(0)) {
			return lhs;
		}
		else if (weight > F(0)) {
			return rhs;
		}
		else {
#if DEBUGGING_FLOAT_ANOMALY
		//assert weight > 0 < 1
#endif
			return lhs * (F(1) - weight) + rhs * weight;
		}
	}

	template<typename T, std::floating_point F>
	LAB_constexpr T Mix(T const& lhs, T const& rhs, F const weight) {
		if (weight < F(0)) {
			return lhs;
		}
		else if (weight > F(0)) {
			return rhs;
		}
		else {
			return lhs * (F(1) - weight) + rhs * weight;
		}
	}
}


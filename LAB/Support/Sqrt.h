#pragma once

#include <concepts>
#include <bit>
#include <cstdint>

#include "../Debugging.h"

namespace lab{
	template<std::floating_point F>
	LAB_constexpr F InverseSqrt(F const input) {
		//copied from wikipedia
#ifdef LAB_MATH_DEBUG
		Debug_Min_Check<F, F(0)>(input);
#endif
		if constexpr(std::is_same_v<F, float>){
			const auto y = std::bit_cast<float>(0x5f3759df - (std::bit_cast<std::uint32_t>(input) >> 1));
			const float refined = y * (1.5f - (input * 0.5f * y * y));
			return refined * (1.5f - (input * 0.5f * refined * refined));
		}
		else if constexpr(std::is_same_v<F, double>){
			const auto y = std::bit_cast<double>(0x5fe6eb50c7b537a9 - (std::bit_cast<std::uint64_t>(input) >> 1));
			const double refined = y * (1.5 - (input * 0.5 * y * y));
			return refined * (1.5 - (input * 0.5 * refined * refined));
		}
		else{
			static_assert(false);
		}
	}

	template<std::floating_point F>
	LAB_constexpr F Sqrt(F const input) {
		return F(1) / InverseSqrt(input);
	}
}
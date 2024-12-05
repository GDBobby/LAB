#pragma once
#include <concepts>
#include <cstdint>

namespace SupportingMath{
	template <std::floating_point F>
	constexpr F Sqrt(F input) {
		
	}
	constexpr float Sqrt(float input) {
		return 0.f;


		//int x_exp = input.get_exponent();
		//int mantissa = bits.get_mantissa();

		//if (bits.is_subnormal) {
		//	++x_exp; //ensure x_exp is the correct exponent of one bit
		//	normalize_float(x_exp, x_mant);
		//}
	}


};


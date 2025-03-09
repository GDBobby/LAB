#pragma once
#include <concepts>
#include <cstdint>

#define USING_CMATH

#ifdef USING_CMATH
#include <cmath>
#else
	#error "alternative math backend not implemented"
#endif


namespace Linear_Algebra{
	namespace SupportingMath {
		template <std::floating_point F>
		constexpr F Sqrt(F input) {
#ifdef USING_CMATH
			return std::sqrt(input);
#else
#endif
		}
		constexpr float Sqrt(float input) {
#ifdef USING_CMATH
			return std::sqrt(input);
#else
#endif


			//int x_exp = input.get_exponent();
			//int mantissa = bits.get_mantissa();

			//if (bits.is_subnormal) {
			//	++x_exp; //ensure x_exp is the correct exponent of one bit
			//	normalize_float(x_exp, x_mant);
			//}
		}
		constexpr double Sqrt(double input) {
#ifdef USING_CMATH
			return std::sqrt(input);
#endif
		}

	}
}


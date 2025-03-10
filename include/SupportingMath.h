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
#ifdef USING_CMATH
		F Sqrt(F const input) {
			return std::sqrt(input);
		}

		template<std::floating_point F>
		F Cos(F const input) {
			return std::cos(input);
		}

		template<std::floating_point F>
		F Sin(F const input) {
			return std::sin(input);
		}
#else
		constexpr F Sqrt(F input) {
			return 0.f;
		}
		constexpr float Sqrt(float input) {
			return 0.f;
		}
		constexpr double Sqrt(double input) {
			return std::sqrt(input);
		}
#endif
	}
}


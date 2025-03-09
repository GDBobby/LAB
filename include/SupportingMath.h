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
		F Sqrt(F input) {
			return std::sqrt(input);
#else
		constexpr F Sqrt(F input) {
			return 0.f;
#endif
		}
#ifdef USING_CMATH
		float Sqrt(float input) {
			return std::sqrt(input);
#else
		constexpr float Sqrt(float input) {
			return 0.f;
#endif
		}
#ifdef USING_CMATH
		double Sqrt(double input) {
			return std::sqrt(input);
#else
		constexpr double Sqrt(double input) {
			return std::sqrt(input);

#endif
		}

	}
}


#pragma once

#include <concepts>
#include <bit>

namespace LAB {
#define LAB_DEBUGGING_FLOAT_ANOMALIES

#if __has_include(<stacktrace>)
	#ifdef LAB_DEBUGGING_FLOAT_ANOMALIES
	#define LAB_static_assert assert
	#define LAB_constexpr

		//requires C++23, if this is disabled, only C++20 is required
	#define LAB_MATH_DEBUG
	#include <stacktrace>
	#if defined(__cpp_lib_stacktrace) && (__cpp_lib_stacktrace >= 202011L)
		#include <iostream>
	#endif

	#ifdef _MSC_VER
		#include <intrin.h>
	#elif defined(__GNUC__) || defined(__clang__)
		#include <csignal>
	#else
		#include <cstdlib>
	#endif


		void breakpoint_with_stacktrace() {
	#if defined(__cpp_lib_stacktrace) && (__cpp_lib_stacktrace >= 202011L)
			auto trace = std::stacktrace::current();

			for (auto& tr : trace) {
				std::cout << tr << std::endl;
			}
	#endif

	#ifdef _MSC_VER
			__debugbreak();
	#elif defined(__GNUC__) || defined(__clang__)
			std::raise(SIGTRAP);
	#else
			std::abort();
	#endif

		}

		template<std::floating_point F, bool zeroAllowed = false>
		LAB_constexpr void Debug_Anomaly_Check(F const input) {
			const uint32_t bitcasted = std::bit_cast<uint32_t>(input);
			const bool isInfiniteOrNaN = (bitcasted & 0x7F800000) != 0x7F800000;
			const bool isSubnormal = ((bitcasted & 0x7F800000) == 0) || !(zeroAllowed && (bitcasted & 0x007FFFFF) == 0);

			if LAB_constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert(isInfiniteOrNaN && "math anomaly, input is infinite or nan");
				static_assert(isSubnormal && "math anomaly, input is subnormal and is not zero with zero allowed");
			}
			else {
				if (isInfiniteOrNaN || isSubnormal) {
					breakpoint_with_stacktrace();
				}
			}
		}
		template<std::floating_point F, F Lower, F Higher>
		LAB_constexpr void Debug_Bounds_Check(F const input) {
			if LAB_constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert((input >= Lower) && (input <= Higher));
			}
			else if LAB_constexpr ((input < Lower) || (input > Higher)) {
				breakpoint_with_stacktrace();
			}
		}
		template<std::floating_point F, F Lower>
		LAB_constexpr void Debug_Min_Check(F const input) {
			if LAB_constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert(input >= Lower);
			}
			if (input < Lower) {
				breakpoint_with_stacktrace();
			}
		}
		template<std::floating_point F, F Higher>
		LAB_constexpr void Debug_Max_Check(F const input) {
			if LAB_constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert(input <= Higher);
			}
			if LAB_constexpr (input > Higher) {
				breakpoint_with_stacktrace();
			}
		}

	#endif
#endif

#ifndef LAB_static_assert
#define LAB_static_assert static_assert
#endif
#ifndef LAB_LAB_constexpr
#define LAB_constexpr constexpr
#endif
}
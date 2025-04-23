#pragma once

namespace lab {
	[[noreturn]] inline void Unreachable() {
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
		__assume(false);
#else // GCC, Clang
		__builtin_unreachable();
#endif
	}
}


//#define LAB_DEBUGGING_FLOAT_ANOMALIES

#ifdef LAB_DEBUGGING_FLOAT_ANOMALIES

#include <concepts>
#include <bit>
//gcc is having issues with this, and im not really sure why or how to fix it. so im just excluding gcc from backtracing
#if !(defined(__GNUC__) && !defined(__clang__))
	#ifdef __has_include
		#if __has_include(<stacktrace>)
			#include <stacktrace>
			#ifdef __cpp_lib_stacktrace
				#if __cpp_lib_stacktrace >= 202011L
					#pragma message("backtrace was included")
					#define LAB_STACK_TRACE_INCLUDED
					#include <iostream>
				#endif
			#endif
		#endif
	#endif
#else
#pragma message("back trace was not included because gcc")
#endif
#ifndef LAB_STACK_TRACE_INCLUDED
#pragma message("backtrace was NOT included")
#endif

	#ifdef _MSC_VER
		#include <intrin.h>
	#elif defined(__GNUC__) || defined(__clang__)
		#include <csignal>
	#else
		#include <cstdlib>
	#endif


	#define LAB_MATH_DEBUG

	#ifndef LAB_static_assert
		#define LAB_static_assert assert
	#endif
	#ifndef LAB_constexpr
		#define LAB_constexpr 
	#endif

#include <cassert>
namespace LAB {

		inline void breakpoint_with_stacktrace() {
	#ifdef LAB_STACK_TRACE_INCLUDED
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
			//fpclassify hasnt been playing nice iwth constexpr, even tho its allegedly constexpr in C++23
			if constexpr (std::is_same_v<F, float>) {
				const uint32_t bitcasted = std::bit_cast<uint32_t>(input);
				const bool isInfiniteOrNaN = (bitcasted & 0x7F800000) == 0x7F800000;
				const bool isSubnormal = ((bitcasted & 0x7F800000) == 0);// || !(zeroAllowed && (bitcasted & 0x007FFFFF) == 0);

				if constexpr(requires { std::integral_constant<F, input>{}; }) {
					static_assert(isInfiniteOrNaN && "math anomaly, input is infinite or nan");
					static_assert(isSubnormal && "math anomaly, input is subnormal and is not zero with zero allowed");
				}
				else {
					if (isInfiniteOrNaN || isSubnormal) {
						breakpoint_with_stacktrace();
					}
				}
			}
			else if constexpr(std::is_same_v<F, double>) {

			}
		}
		template<std::floating_point F, F Lower, F Higher>
		LAB_constexpr void Debug_Bounds_Check(F const input) {
			if constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert((input >= Lower) && (input <= Higher));
			}
			else if ((input < Lower) || (input > Higher)) {
				breakpoint_with_stacktrace();
			}
		}
		template<std::floating_point F, F Lower>
		LAB_constexpr void Debug_Min_Check(F const input) {
			if constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert(input >= Lower);
			}
			if (input < Lower) {
				breakpoint_with_stacktrace();
			}
		}
		template<std::floating_point F, F Higher>
		LAB_constexpr void Debug_Max_Check(F const input) {
			if constexpr (requires { std::integral_constant<F, input>{}; }) {
				static_assert(input <= Higher);
			}
			if (input > Higher) {
				breakpoint_with_stacktrace();
			}
		}

}
#endif

#ifndef LAB_static_assert
#define LAB_static_assert static_assert
#endif
#ifndef LAB_constexpr
//this is constexpr unless LAB_debugging is enabled, in which case its nothing
#define LAB_constexpr constexpr
#endif
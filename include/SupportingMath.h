#pragma once
#include <concepts>
#include <cstdint>
#include <numbers>
#include <bit>

//#define USING_CMATH

#ifdef USING_CMATH
#include <cmath>
#else
#include "qt_sine_table.h"
#endif

//#define DEBUGGING_FLOAT_ANOMALIES

#ifdef __cpp_lib_stacktrace
#ifdef DEBUGGING_FLOAT_ANOMALIES
#define MATH_DEBUG
#include <stacktrace>

#ifdef _MSC_VER
	#include <intrin.h>
#elif defined(__GNUC__) || defined(__clang__)
	#include <csignal>
#else
	#include <cstdlib>
#endif

void BREAKPOINT() {
	auto trace = std::stacktrace::current(1, 5);
	for(auto& tr : trace){
		std::cout << tr << std::endl;
	}

	#ifdef _MSC_VER
		__debugbreak()
	#elif defined(__GNUC__) || defined(__clang__)
		std::raise(SIGTRAP)
	#else
		std::abort()
	#endif
}

#endif
#endif



namespace LAB{
	namespace SupportingMath {
		template<std::floating_point F>
		inline constexpr F PI = std::numbers::pi_v<F>;

		template<std::floating_point F, bool Inverse = false>
		static constexpr F GetPI(F multiplier) {
			if constexpr(Inverse){
				return multiplier / PI<F>;
			}
			else{
				return multiplier * PI<F>;
			}
		}
		template<std::floating_point F>
		static constexpr F GetPI_DividedBy(F divisor){
			return PI<F> / divisor;
		}

#ifdef USING_CMATH
		template <std::floating_point F>
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
		template<std::floating_point F>
		constexpr F Abs(F const input){
			return input * (F(1) - F(2) * (input < F(0)));
		}

		template<std::floating_point F>
		constexpr F Trunc(F const input){
#if MATH_DEBUGGING
			if (input == std::nanf())


			}
#endif
			
			if constexpr(std::is_same_v<F, float>){
				const uint32_t bits = std::bit_cast<uint32_t>(input);

				//ill add debugging methods later

				// If x == ±∞ then return num
				// If x == ±NaN then return num
				//if (CCM_UNLIKELY(bits.is_inf_or_nan())) { return num; }

				// If x == ±0 then return num
				//if (CCM_UNLIKELY(num == 0.0)) { return num; }

				//255 is the first 8 bits set to true
				 //23 is mantissa length
				constexpr int exponent_mask = 255 << 23;
				const int exponent = (bits & exponent_mask) >> 23;

				// If the exponent is greater than or equal to the fraction length, then we will return the number as is since it is already an integer.
				//fraction length is 23, equal to the amount of mantissa bits
				if (exponent >= 23) { return input; }

				// If our exponent is set up such that the abs(x) is less than 1 we will instead return 0.
				if (exponent <= -1) { return 0.f; }

				// Perform the truncation
				const int trimming_size = 23 - exponent;

				constexpr uint32_t mantissa_mask = (1 << 23) - 1;
				const auto truncated_mantissa = static_cast<uint32_t>(((bits & mantissa_mask) >> trimming_size) << trimming_size);

				constexpr uint32_t mantissa_anti_mask = UINT32_MAX ^ mantissa_mask;
				return std::bit_cast<float>((bits & mantissa_anti_mask) | truncated_mantissa);
			}
		}
		template<std::floating_point F>
		constexpr F Mod(F const x, F const y){
			//stealing from ccmath a little bit
			return static_cast<F>(x - (Trunc(x / y) * y));
		}

		template<std::floating_point F>
		constexpr F InverseSqrt(F const input){
			//copied from wikipedia
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
		constexpr F Sqrt(F const input) {
			return F(1) / InverseSqrt(input);
		}

		//im literally just ripping this from QT
		//https://codebrowser.dev/qt5/qtbase/src/corelib/kernel/qmath.cpp.html#qt_sine_table

		template<std::floating_point F>
		constexpr F Sin(F const input){
			int si = int(input * (F(0.5) * QT_Sine_Table::size / std::numbers::pi_v<F>)); // Would be more accurate with qRound, but slower.
			const F d = input - F(si) * (F(2.0) * std::numbers::pi_v<F> / QT_Sine_Table::size);
			int ci = si + QT_Sine_Table::size/ 4;
			si &= QT_Sine_Table::size - 1;
			ci &= QT_Sine_Table::size - 1;
			if constexpr(std::is_same_v<F, float>){
				return QT_Sine_Table::table_float[si] + (QT_Sine_Table::table_float[ci] - 0.5f * QT_Sine_Table::table_float[si] * d) * d;
			}
			else if constexpr(std::is_same_v<F, double>){
				return QT_Sine_Table::table_double[si] + (QT_Sine_Table::table_double[ci] - 0.5 * QT_Sine_Table::table_double[si] * d) * d;
			}
			else{
				static_assert(false);
			}
		}

		template<std::floating_point F>
		constexpr F Cos(F const input) {
			int si = int(input * (F(0.5) * QT_Sine_Table::size / std::numbers::pi_v<F>)); // Would be more accurate with qRound, but slower.
			const F d = input - F(si) * (F(2.0) * std::numbers::pi_v<F> / QT_Sine_Table::size);
			int ci = si + QT_Sine_Table::size / 4;
			si &= QT_Sine_Table::size - 1;
			ci &= QT_Sine_Table::size - 1;
			if constexpr(std::is_same_v<F, float>){
				return QT_Sine_Table::table_float[ci] - (QT_Sine_Table::table_float[si] + 0.5f * QT_Sine_Table::table_float[ci] * d) * d;
			}
			else if constexpr(std::is_same_v<F, double>){
				return QT_Sine_Table::table_double[ci] - (QT_Sine_Table::table_double[si] + 0.5 * QT_Sine_Table::table_double[ci] * d) * d;
			}
			else{
				static_assert(false);
			}
		}

		template<std::floating_point F>
		constexpr F Tan(F const input){
			//return Sin(input) / Cos(input);

			int si = int(input * (F(0.5) * QT_Sine_Table::size / PI<F>)); // Would be more accurate with qRound, but slower.
			const F d = input - F(si) * (F(2.0) * PI<F> / QT_Sine_Table::size);
			int ci = si + QT_Sine_Table::size / 4;
			si &= QT_Sine_Table::size - 1;
			ci &= QT_Sine_Table::size - 1;
			if constexpr(std::is_same_v<F, float>){
				return (QT_Sine_Table::table_float[si] + (QT_Sine_Table::table_float[ci] - 0.5f * QT_Sine_Table::table_float[si] * d) * d)
				/ (QT_Sine_Table::table_float[ci] - (QT_Sine_Table::table_float[si] + 0.5f * QT_Sine_Table::table_float[ci] * d) * d);
			}
			else if constexpr(std::is_same_v<F, double>){
				return (QT_Sine_Table::table_double[si] + (QT_Sine_Table::table_double[ci] - 0.5f * QT_Sine_Table::table_double[si] * d) * d)
				/ (QT_Sine_Table::table_double[ci] - (QT_Sine_Table::table_double[si] + 0.5f * QT_Sine_Table::table_double[ci] * d) * d);
			}
			else{
				static_assert(false);
			}
		}


		template<std::floating_point F>
		constexpr F ArcCos(F const input){
			//https://developer.download.nvidia.com/cg/acos.html
			const F negate = F(input < 0);
			const F absInput = Abs(input);
			F ret = F(-0.0187293);
			ret *= absInput;
			ret += F(0.0742610);
			ret *= absInput;
			ret -= F(0.2121144);
			ret *= absInput;
			ret += F(1.5707288);
			ret *= Sqrt(F(1.0) - absInput);
			ret -= F(2) * negate * ret;
			return negate * PI<F> + ret;
		}
		template<std::floating_point F>
		constexpr F ArcSin(F const input){
			//https://developer.download.nvidia.com/cg/index_stdlib.html
			const F negate = F(input < 0);
			const F absInput = Abs(input);
			F ret = F(-0.0187293);
			ret *= absInput;
			ret += F(0.0742610);
			ret *= absInput;
			ret -= F(0.2121144);
			ret *= absInput;

			ret += F(1.5707288);
			ret = GetPI(F(0.5)) - Sqrt(F(1.0) - absInput) * ret;
			return ret - F(2) * negate * ret;
		}
		template<std::floating_point F>
		constexpr F ArcTan(F const input){
			//https://developer.download.nvidia.com/cg/atan.html
			const F absX = Abs(input);
			const F absY = F(1);
			const bool yBigger = absY > absX;
			const F minOverMax = absX / absY * F(yBigger) + absY / absX * F(!yBigger);

			return minOverMax;
		}
		template<std::floating_point F>
		constexpr F ArcTan2(F const x, F const y){
			//https://developer.download.nvidia.com/cg/atan2.html
			//heavily paraphrased, they had to be trolling

			const F absX = Abs(x);
			const F absY = Abs(y);

			const bool yBigger = absY > absX;
			//const F minOverMax = Min(absX, absY) / Max(absX, absY);
			const F minOverMax = absX / absY * F(yBigger) + absY / absX * F(!yBigger);

			const F t4 = minOverMax * minOverMax;
			F t0 = 		  -F(0.013480470);
			t0 = t0 * t4 + F(0.057477314);
			t0 = t0 * t4 - F(0.121239071);
			t0 = t0 * t4 + F(0.195635925);
			t0 = t0 * t4 - F(0.332994597);
			t0 = t0 * t4 + F(0.999995630);
			F t3 = t0 * minOverMax;

			
			t3 = (yBigger) ? GetPI(F(0.5)) - t3 : t3;
			t3 = (x < F(0)) ?  PI<F> - t3 : t3;
			t3 = (y < F(0)) ? -t3 : t3;
			/*
			t3 = t3 * (F(1) - F(2) * (y < F(0)))
				+ (yBigger * (GetPI(F(0.5)) - F(2) * t3))
				+ (x < F(0)) * (F(2) * (PI<F> - t3));
			*/
			return t3;
		}

/*
		//ArcTan2BitMasking lost in a microbenchmark against ArcTan
		template<std::floating_point F>
		constexpr F ArcTan2BitMasking(F const x, F const y){
			//https://developer.download.nvidia.com/cg/atan2.html
			//heavily paraphrased, they had to be trolling

			const F absX = Abs(x);
			const F absY = Abs(y);

			const bool yBigger = absY > absX;
			//const F minOverMax = Min(absX, absY) / Max(absX, absY);
			const F minOverMax = absX / absY * F(yBigger) + absY / absX * F(!yBigger);

			const F t4 = minOverMax * minOverMax;
			F t0 = 		  -F(0.013480470);
			t0 = t0 * t4 + F(0.057477314);
			t0 = t0 * t4 - F(0.121239071);
			t0 = t0 * t4 + F(0.195635925);
			t0 = t0 * t4 - F(0.332994597);
			t0 = t0 * t4 + F(0.999995630);
			F t3 = t0 * minOverMax;

			t3 = t3 * (F(1) - F(2) * (y < F(0)))
				+ (yBigger * (GetPI(F(0.5)) - F(2) * t3))
				+ (x < F(0)) * (F(2) * (PI<F> - t3));

			return t3;
		}
*/

		template<std::floating_point F>
		constexpr F DegreesToRadians(F const theta){
			return theta * GetPI<F, true>(F(180));
		}

		template<std::floating_point F>
		constexpr F RadiansToDegrees(F const theta){
			return theta * GetPI_DividedBy(F(180)); 
		}
#endif
	}
}


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


namespace LAB{
	namespace SupportingMath {
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
				return QT_Sine_Table::table_float[si] - (QT_Sine_Table::table_float[ci] + 0.5f * QT_Sine_Table::table_float[si] * d) * d;
			}
			else if constexpr(std::is_same_v<F, double>){
				return QT_Sine_Table::table_double[si] - (QT_Sine_Table::table_double[ci] + 0.5 * QT_Sine_Table::table_double[si] * d) * d;
			}
			else{
				static_assert(false);
			}
		}

		template<std::floating_point F>
		constexpr F Tan(F const input){
			return Sin(input) / Cos(input);
		}

		template<std::floating_point F>
		constexpr F DegreesToRadians(F const theta){
			return theta * F(180) / std::numbers::pi_v<F>;
		}

		template<std::floating_point F>
		constexpr F RadiansToDegrees(F const theta){
			return theta / F(180) * std::numbers::pi_v<F>; 
		}
#endif
	}
}


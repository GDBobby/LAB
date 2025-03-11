#pragma once
#include <concepts>
#include <cstdint>
#include <numbers>

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
		constexpr F Sqrt(F const input) {
			return input;
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
#endif
	}
}


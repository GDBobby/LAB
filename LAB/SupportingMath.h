#pragma once
#include <concepts>
#include <cstdint>
#include <bit>

//#define USING_CMATH

#include "qt_sine_table.h"

#include "Debugging.h"


namespace lab {
	template<std::floating_point F>
	inline constexpr F PI = F(3.1415926535);

	template<std::floating_point F, bool Inverse = false>
	static LAB_constexpr F GetPI(F multiplier) {
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(multiplier);
#endif
		if constexpr(Inverse){
			return multiplier / PI<F>;
		}
		else{
			return multiplier * PI<F>;
		}
	}
	template<std::floating_point F>
	static LAB_constexpr F GetPI_DividedBy(F divisor) {
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check(divisor);
#endif
		return PI<F> / divisor;
	}

	

	template<typename T>
	LAB_constexpr T Max(T const a, T const b){
		const bool aBigger = a > b;
		return a * aBigger + b * !aBigger;
	}
	template<typename T>
	LAB_constexpr T Min(T const a, T const b){
		const bool aSmaller = a < b;
		return a * aSmaller + b * !aSmaller;
	}


	template<std::floating_point F>
	LAB_constexpr F Abs(F const input) {
		return input * (F(1) - F(2) * (input < F(0)));
	}

	template<std::floating_point F>
	LAB_constexpr F Trunc(F const input) {
		//stealing from ccmath a little bit
		//https://github.com/Rinzii/ccmath
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check(input);
#endif
		
		if constexpr(std::is_same_v<F, float>) {
			const uint32_t bits = std::bit_cast<uint32_t>(input);

			//ill add debugging methods later

			// If x == ±∞ then return num
			// If x == ±NaN then return num
			//if (CCM_UNLIKELY(bits.is_inf_or_nan())) { return num; }

			// If x == ±0 then return num
			//if (CCM_UNLIKELY(num == 0.0)) { return num; }

			//255 is the first 8 bits set to true
			//23 is mantissa length

			const int exponent = ((bits >> 23) & 0xFF) - 127;

			if (exponent >= 23) {
				return input;  // Already an integer
			}
			if (exponent < 0) {
				return 0.f;
			}

			// Perform the truncation
			const int trimming_size = 23 - (exponent);

			LAB_constexpr uint32_t mantissa_mask = (1 << 23) - 1;
			const auto truncated_mantissa = static_cast<uint32_t>(((bits & mantissa_mask) >> trimming_size) << trimming_size);

			LAB_constexpr uint32_t mantissa_anti_mask = UINT32_MAX ^ mantissa_mask;
			return std::bit_cast<float>((bits & mantissa_anti_mask) | truncated_mantissa);
		}
		else if constexpr (std::is_same_v<F, double>) {


			assert(false);
			return 0.0;
		}
	}
	template<std::floating_point F>
	LAB_constexpr F Mod(F const x, F const y) {
		//stealing from ccmath a little bit
		//https://github.com/Rinzii/ccmath
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(x);
		Debug_Anomaly_Check(y);
#endif
		
		//https://quick-bench.com/q/SzfiLPh2admpveSGNjQrUV5D6Qg
		return x - (Trunc(x / y) * y);
	}

	template<std::floating_point F>
	LAB_constexpr F PhaseTo(F const input, F const lower, F const higher) {

		F const fullRange = higher - lower;
		F const moddedInput = Mod(input - lower, fullRange) + lower;
		if (moddedInput > higher) {
			return moddedInput - fullRange;
		}
		return moddedInput;
	}

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

	//im literally just ripping this from QT
	//https://codebrowser.dev/qt5/qtbase/src/corelib/kernel/qmath.cpp.html#QT_Sine

	template<std::floating_point F>
	LAB_constexpr F Sin(F const input) {
		int si = int(input * (F(0.5) * QT_Sine::size / PI<F>)); // Would be more accurate with qRound, but slower.
		const F d = input - F(si) * (F(2.0) * PI<F> / QT_Sine::size);
		int ci = si + QT_Sine::size/ 4;
		si &= QT_Sine::size - 1;
		ci &= QT_Sine::size - 1;
		return QT_Sine::table<F>[si] + (QT_Sine::table<F>[ci] - F(0.5) * QT_Sine::table<F>[si] * d) * d;
	}

	template<std::floating_point F>
	LAB_constexpr F Cos(F const input) {
		int si = int(input * (F(0.5) * QT_Sine::size / PI<F>)); // Would be more accurate with qRound, but slower.
		const F d = input - F(si) * (F(2.0) * PI<F> / QT_Sine::size);
		int ci = si + QT_Sine::size / 4;
		si &= QT_Sine::size - 1;
		ci &= QT_Sine::size - 1;
		return QT_Sine::table<F>[ci] - (QT_Sine::table<F>[si] + F(0.5) * QT_Sine::table<F>[ci] * d) * d;
	}

	template<std::floating_point F>
	LAB_constexpr F Tan(F const input) {
		//return Sin(input) / Cos(input);

		int si = int(input * (F(0.5) * QT_Sine::size / PI<F>)); // Would be more accurate with qRound, but slower.
		const F d = input - F(si) * (F(2.0) * PI<F> / QT_Sine::size);
		int ci = si + QT_Sine::size / 4;
		si &= QT_Sine::size - 1;
		ci &= QT_Sine::size - 1;
		return (QT_Sine::table<F>[si] + (QT_Sine::table<F>[ci] - F(0.5) * QT_Sine::table<F>[si] * d) * d)
			/ (QT_Sine::table<F>[ci] - (QT_Sine::table<F>[si] + F(0.5) * QT_Sine::table<F>[ci] * d) * d);
		
	}


	template<std::floating_point F>
	LAB_constexpr F ArcCos(F const input) {
		//https://developer.download.nvidia.com/cg/acos.html

#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(input);
		Debug_Bounds_Check<F, F(-1), F(1)>(input);
#endif

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
	LAB_constexpr F ArcSin(F const input) {
		//https://developer.download.nvidia.com/cg/index_stdlib.html
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(input);
		Debug_Bounds_Check<F, F(-1), F(1)>(input);
#endif
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
	LAB_constexpr F ArcTan(F const y) {
#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(y);
#endif
		//https://developer.download.nvidia.com/cg/atan.html
		const F absX = F(1);
		const F absY = Abs(y);
		const bool yBigger = absY > absX;
		const F minOverMax = absX / absY * F(yBigger) + absY / absX * F(!yBigger);
		const F t4 = minOverMax * minOverMax;
		F t0 = -F(0.013480470);
		t0 = t0 * t4 + F(0.057477314);
		t0 = t0 * t4 - F(0.121239071);
		t0 = t0 * t4 + F(0.195635925);
		t0 = t0 * t4 - F(0.332994597);
		t0 = t0 * t4 + F(0.999995630);
		F t3 = t0 * minOverMax;


		t3 = (yBigger) ? GetPI(F(0.5)) - t3 : t3;
		return (y < F(0)) ? -t3 : t3;
	}
	template<std::floating_point F>
	LAB_constexpr F ArcTan2(F const y, F const x){
		//https://developer.download.nvidia.com/cg/atan2.html
		//heavily paraphrased, they had to be trolling

#ifdef LAB_MATH_DEBUG
		Debug_Anomaly_Check<F, true>(y);
		Debug_Anomaly_Check<F, true>(x);
#endif
		//i think im missing an exception case

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
		/*
		t3 = t3 * (F(1) - F(2) * (y < F(0)))
			+ (yBigger * (GetPI(F(0.5)) - F(2) * t3))
			+ (x < F(0)) * (F(2) * (PI<F> - t3));
		*/
		return (y < F(0)) ? -t3 : t3;
	}

/*
	//ArcTan2BitMasking lost in a microbenchmark against ArcTan. i think in some situations its slower, and in full random situations its even. might require a revisit
	template<std::floating_point F>
	LAB_constexpr F ArcTan2BitMasking(F const x, F const y){
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
	LAB_constexpr F DegreesToRadians(F const theta){
		return theta * GetPI<F, true>(F(180));
	}

	template<std::floating_point F>
	LAB_constexpr F RadiansToDegrees(F const theta){
		return theta * GetPI_DividedBy(F(180)); 
	}
}


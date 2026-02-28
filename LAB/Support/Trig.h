#pragma once

#include <concepts>

//#define USING_CMATH

#include "../Debugging.h"
#include "Simple.h"


#if defined(LAB_USING_SSE) || defined(LAB_USING_AVX2)
#define USING_SIMD
#include <immintrin.h>
#endif

namespace lab {

	template<std::floating_point F>
	LAB_constexpr F PhaseTo(F const input, F const lower, F const higher) {

		F const fullRange = higher - lower;
		F const moddedInput = Mod(input - lower, fullRange) + lower;
		if (moddedInput > higher) {
			return moddedInput - fullRange;
		}
		return moddedInput;
	}


	//chebyshev implementation
	template<std::floating_point F>
	LAB_constexpr F Sin(F const input) {
		F phased = lab::PhaseTo(input, -GetPI_DividedBy(F(2)), GetPI(F(1.5)));

		if(phased > GetPI_DividedBy(F(2))){
			phased = PI<F> - phased;
		}

		const F pow_val = phased * phased;
		return phased * (F(1.0) 
			+ pow_val * (F(-0.16666666666666666) + 
				pow_val * (F(0.00833333333333333) + 
					pow_val * (F(-0.00019841269841269) + 
						pow_val * (F(0.00000275573192239))
					)
				)
			)
		);
	}

	template<std::floating_point F>
	LAB_constexpr F Cos(F const input) {
		return Sin(input + GetPI_DividedBy(2.f));
	}

	template<std::floating_point F>
	LAB_constexpr F Tan(F const input) {
		//return Sin(input) / Cos(input);

		constexpr F half_pi = GetPI_DividedBy(F(2));
		constexpr F quarter_pi = GetPI_DividedBy(F(4));

		F phased = PhaseTo(input, -half_pi, half_pi);

		bool reflected = false;
		if(phased > quarter_pi){
			reflected = true;
			phased = half_pi - phased;
		}
		else if (phased < -quarter_pi){
			reflected = true;
			phased = -half_pi - phased;
		}

		const F p2 = phased * phased;
		F result = phased * (F(1.0) + p2 * (F(0.33333333333) + 
                    p2 * (F(0.13333333337) + 
                    p2 * (F(0.0539682539) + 
                    p2 * (F(0.0218694885) + 
                    p2 * (F(0.0088632355) + 
                    p2 * (F(0.0035920791))))))));

		if (reflected) {
			return F(1.0) / result;
		}

		return result;
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
}


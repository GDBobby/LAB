#pragma once
	
#include <concepts>

#include "../Debugging.h"


namespace lab{
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

	template<std::floating_point F>
	inline constexpr F epsilon = F(1e-6); //potentially bigger for double?
	

	template<std::floating_point F>
	LAB_constexpr F DegreesToRadians(F const theta){
		return theta * GetPI_DividedBy(F(180));
	}

	template<std::floating_point F>
	LAB_constexpr F RadiansToDegrees(F const theta){
		return theta * GetPI<F, true>(F(180)); 
	}
}
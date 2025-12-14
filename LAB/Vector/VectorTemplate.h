#pragma once

#include "../SupportingMath.h"
#include "../Debugging.h"

#include <cstdint>
#include <concepts>
//#if debug?
#include <cassert>
//#endif debug?



namespace lab {
	//F short for floating point, can be double or float, Dimensions is short for dimensions
	template<std::floating_point F, uint8_t Dimensions> 
		requires((Dimensions > 1) && (Dimensions <= 4))
	struct Vector{};
}


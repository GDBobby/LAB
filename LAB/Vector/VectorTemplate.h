#pragma once

#include "../Support/Generic.h"
#include "../Support/Sqrt.h"
#include "../Debugging.h"

#include <cstdint>
#include <concepts>
#if LAB_DEBUG_LEVEL > 0
#include <cassert>
#endif



namespace lab {
	//F short for floating point, can be double or float, Dimensions is short for dimensions
	template<std::floating_point F, uint8_t Dimensions> 
	requires((Dimensions > 1) && (Dimensions <= 4))
	struct Vector{};
}


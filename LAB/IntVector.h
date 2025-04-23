#pragma once

#include <cstdint>
#include <concepts>

#include <immintrin.h>

namespace lab {
	template<std::integral F, uint8_t Dimensions> 
	requires((Dimensions > 1) && (Dimensions <= 4))
	struct IntVector{};

    
	template<std::integral I>
	struct IntVector<I, 2> {
        I x;
        I y;
		LAB_constexpr IntVector() {}
		LAB_constexpr IntVector(I const x, I const y) : x{ x }, y{ y } {}
		LAB_constexpr IntVector(I const all) : x{ all }, y{ all } {}
    };
	template<std::integral I>
	struct IntVector<I, 3> {
        I x;
        I y;
        I z;
		LAB_constexpr IntVector() {}
		LAB_constexpr IntVector(I const x, I const y, I const z) : x{ x }, y{ y }, z{z} {}
		LAB_constexpr IntVector(I const all) : x{ all }, y{ all }, z{all} {}
    };
	template<std::integral I>
	struct IntVector<I, 4> {
        I x;
        I y;
        I z;
        I w;
		LAB_constexpr IntVector() {}
		LAB_constexpr IntVector(I const x, I const y, I const z, I const w) : x{ x }, y{ y }, z{z}, w{w} {}
		LAB_constexpr IntVector(I const all) : x{ all }, y{ all }, z{all}, w{all} {}
    };

    using ivec2 = IntVector<int, 2>;
    using ivec3 = IntVector<int, 3>;
    using ivec4 = IntVector<int, 4>;
}
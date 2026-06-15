#pragma once

#include <cstdint>
#include <concepts>
#include <utility>

namespace lab {
	template<std::integral F, uint8_t Dimensions> 
	requires((Dimensions > 1) && (Dimensions <= 4))
	struct IntVector{};

    
	template<std::integral I>
	struct IntVector<I, 2> {
        I x;
        I y;
		LAB_constexpr IntVector() {}
		LAB_constexpr IntVector(I const _x, I const _y) : x{ _x }, y{ _y } {}
		LAB_constexpr IntVector(I const all) : x{ all }, y{ all } {}

		I& operator[](uint8_t index){
			switch(index){
				case 0: return x;
				case 1: return y;
				default: std::unreachable();
			}
			std::unreachable();
		}
		I operator[](uint8_t index) const {
			switch(index){
				case 0: return x;
				case 1: return y;
				default: std::unreachable();
			}
			std::unreachable();
		}
    };
	template<std::integral I>
	struct IntVector<I, 3> {
        I x;
        I y;
        I z;
		LAB_constexpr IntVector() {}
		LAB_constexpr IntVector(I const _x, I const _y, I const _z) : x{ _x }, y{ _y }, z{_z} {}
		LAB_constexpr IntVector(I const all) : x{ all }, y{ all }, z{all} {}
    
		I& operator[](uint8_t index){
			switch(index){
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: std::unreachable();
			}
			std::unreachable();
		}
		I operator[](uint8_t index) const{
			switch(index){
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: std::unreachable();
			}
			std::unreachable();
		}
	};
	template<std::integral I>
	struct IntVector<I, 4> {
        I x;
        I y;
        I z;
        I w;
		LAB_constexpr IntVector() {}
		LAB_constexpr IntVector(I const _x, I const _y, I const _z, I const _w) : x{ _x }, y{ _y }, z{_z}, w{_w} {}
		LAB_constexpr IntVector(I const all) : x{ all }, y{ all }, z{all}, w{all} {}

		I& operator[](uint8_t index){
			switch(index){
				case 0: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
				default: std::unreachable();
			}
			std::unreachable();
		}
		I operator[](uint8_t index) const {
			switch(index){
				case 0: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
				default: std::unreachable();
			}
			std::unreachable();
		}
    };

    using ivec2 = IntVector<int, 2>;
    using ivec3 = IntVector<int, 3>;
    using ivec4 = IntVector<int, 4>;
}
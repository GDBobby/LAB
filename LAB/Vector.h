#pragma once


#include "Vector/Vector2.h"
#include "Vector/Vector3.h"
#include "Vector/Vector4f.h"
#include "Vector/Vector4SIMD.h"
#include "Vector/Vector4d.h"
#include "Vector/IntVector.h"

namespace lab{


    template<std::floating_point F, uint8_t Dimensions>
    Vector<F, Dimensions> operator*(F const f, Vector<F, Dimensions> const vec) {
        return vec * f;
    }
    template<std::floating_point F, uint8_t Dimensions>
    Vector<F, Dimensions> operator+(F const f, Vector<F, Dimensions> const vec) {
        return vec + f;
    }
    //minus and divide operators are per dimension since order of operations matters
	template<std::floating_point F, uint8_t Dimensions>
    Vector<F, Dimensions> operator/(F const f, Vector<F, 3> const vec){
		if constexpr(Dimensions == 2){
			return Vector<F, 2>{
				f / vec.x,
				f / vec.y
			};
		}
		if constexpr(Dimensions == 3){
			return Vector<F, 3>{
				f / vec.x,
				f / vec.y,
				f / vec.z
			};
		}
		if constexpr(Dimensions == 4){
			return Vector<F, 4>{
				f / vec.x,
				f / vec.y,
				f / vec.z,
				f / vec.w
			};
		}
    }
    
    template<std::floating_point F, uint8_t Dimensions>
    Vector<F, 3> operator-(F const f, Vector<F, 3> const vec){
		if constexpr(Dimensions == 2){
			return Vector<F, 2>{
				f - vec.x,
				f - vec.y
			};
		}
		if constexpr(Dimensions == 3){
			return Vector<F, 3>{
				f - vec.x,
				f - vec.y,
				f - vec.z
			};
		}
		if constexpr(Dimensions == 4){
			return Vector<F, 4>{
				f - vec.x,
				f - vec.y,
				f - vec.z
			};
		}
    }






    template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F Dot(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		return first.Dot(second);
	}
	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr Vector<F, Dimensions> Normalized(Vector<F, Dimensions> const vec){
		return vec.Normalized();
	}

	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr auto MagnitudeSqrd(Vector<F, Dimensions> const& vec){
		return vec.SquaredMagnitude();
	}
	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F Magnitude(Vector<F, Dimensions> const& vec){
		return vec.Magnitude();
	}

	//according to my testing, this is 28% faster than a conventional Dot(Normalize, Normalize)
    //it saves a single sqrt call
	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F NormalizedDot(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		const F combinedMagSquared = first.SquaredMagnitude() * second.SquaredMagnitude();
        const F numerator = first.Dot(second);
        if(numerator > F(0)){
            return numerator * InverseSqrt(combinedMagSquared);
        }
        else if (numerator < F(0)){
            return -numerator * InverseSqrt(combinedMagSquared);
        }
        else{
            //i should get LAB_DEBUG in here
            return F(0);
        }
	}


	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr auto Cross(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		if constexpr(Dimensions == 2){
			return first.x * second.y - first.y * second.x;
		}
		else if constexpr(Dimensions == 3){
			return Vector<F, 3>{
				first.y * second.z - first.z * second.y,
				first.z * second.x - first.x * second.z,
				first.x * second.y - first.y * second.x
			};
		}
	}

	//look up https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process for the 4th dimensional Orthogonal

	using vec2 = Vector<float, 2>;
	using vec3 = Vector<float, 3>;
	using vec4 = Vector<float, 4>;
	using vec2f = Vector<float, 2>;
	using vec3f = Vector<float, 3>;
	using vec4f = Vector<float, 4>;
	using vec2d = Vector<double, 2>;
	using vec3d = Vector<double, 3>;
	using vec4d = Vector<double, 4>;
}
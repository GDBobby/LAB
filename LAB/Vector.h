#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4f.h"
#include "Vector4d.h"

namespace lab{
    template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F Dot(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		return first.Dot(second);
	}
	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr Vector<F, Dimensions> Normalized(Vector<F, Dimensions> const vec){
		return vec.Normalized();
	}

	//according to my testing, this is 28% faster than a conventional Dot(Normalize, Normalize)
    //it saves a single sqrt call
	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F NormalizedDot(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		const F combinedMagSquared = first.SquaredMagnitude() * second.SquaredMagnitude();
        const F numerator = first.Dot(second);
        if(numerator > F(0)){
            return numerator * SupportingMath::InverseSqrt(combinedMagSquared);
        }
        else if (numerator < F(0)){
            return -numerator * SupportingMath::InverseSqrt(combinedMagSquared);
        }
        else{
            //i should get LAB_DEBUG in here
            return F(0);
        }
	}


	template<std::floating_point F>
	LAB_constexpr F Cross(Vector<F, 2> const first, Vector<F, 2> const second) {
		return first.x * second.y - first.y * second.x;
	}

	template<std::floating_point F>
	LAB_constexpr Vector<F, 3> Cross(Vector<F, 3> const first, Vector<F, 3> const second) {
		//inlining matrix
		return Vector<F, 3>{
			first.y * second.z - first.z * second.y,
			first.z * second.x - first.x * second.z,
			first.x * second.y - first.y * second.x
		};
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
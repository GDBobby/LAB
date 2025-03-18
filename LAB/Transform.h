#pragma once
#include "Vector.h"
#include "Matrix.h"

#include <concepts>


namespace LAB {

    template<std::floating_point F, std::uint8_t Dimensions> 
	requires((Dimensions > 1) && (Dimensions <= 4))
    struct Transform{};

    template<std::floating_point F>
    struct Transform<F, 2> {
        Vector<F, 2> translation;
        Vector<F, 2> scale;
        F rotation;

		//Transform() {}
        LAB_constexpr Transform() : translation{F(0)}, scale{F(1)}, rotation{F(0)} {}
        LAB_constexpr Transform(Vector<F, 2> const translation) : translation{translation}, scale{F(1)}, rotation{F(0)}{}
        LAB_constexpr Transform(Vector<F, 2> const translation, Vector<F, 2> const scale) : translation{translation}, scale{scale}, rotation{F(0)}{}
        LAB_constexpr Transform(Vector<F, 2> const translation, Vector<F, 2> const scale, F const rotation) : translation{translation}, scale{scale}, rotation{rotation}{}

    
		template<uint8_t Alignment = 4> requires(Alignment >= 3)
		LAB_constexpr Matrix<F, 3, 3, Alignment> ToMatrix() const{
			const float cosine = SupportingMath::Cos(rotation);
			const float sine = SupportingMath::Sin(rotation);
			Matrix<F, 3, 3, Alignment> ret;
			ret.At(0, 0) = scale.x * cosine;
			ret.At(0, 1) = scale.x * sine;
			ret.At(0, 2) = 0.f;

			ret.At(1, 0) = scale.y * -sine;
			ret.At(1, 1) = scale.y * cosine;
			ret.At(1, 2) = 0.f;

			ret.At(2, 0) = translation.x;
			ret.At(2, 1) = translation.y;
			ret.At(2, 2) = 1.f;

			return ret;
		}
	
		template<uint8_t Alignment = 4> requires(Alignment >= 3)
		LAB_constexpr Matrix<F, 3, 3, Alignment> ToMatrixNoRotation() const {
			//const float cosine = glm::cos(rotation);
			//const float sine = glm::sin(rotation);
			Matrix<F, 3, 3, Alignment> ret{F(0)};
			ret.At(0, 0) = scale.x;

			ret.At(1, 1) = scale.y;

			ret.At(2, 0) = translation.x;
			ret.At(2, 1) = translation.y;
			ret.At(2, 2) = 1.f;

        	return ret;
    	}
    };

    template<std::floating_point F>
    struct Transform<F, 3> {
        Vector<F, 3> translation;
        Vector<F, 3> scale;
        Vector<F, 3> rotation;


		LAB_constexpr Transform() : translation{ F(0) }, scale{ F(1) }, rotation{ F(0) } {}
		LAB_constexpr Transform(Vector<F, 3> const translation) : translation{ translation }, scale{ F(1) }, rotation{ F(0) } {}
		LAB_constexpr Transform(Vector<F, 3> const translation, Vector<F, 3> const scale) : translation{ translation }, scale{ scale }, rotation{ F(0) } {}
		LAB_constexpr Transform(Vector<F, 3> const translation, Vector<F, 3> const scale, Vector<F, 3> const rotation) : translation{ translation }, scale{ scale }, rotation{ rotation } {}

		template<uint8_t Alignment = 4> requires(Alignment >= 4)
		LAB_constexpr Matrix<F, 4, 4, Alignment> GetRotationXMatrix() const {
			Matrix<F, 4, 4, Alignment> ret{ F(0) };

			ret.At(0, 0) = F(1);
			ret.At(3, 3) = F(1);

			const F sinRet = SupportingMath::Sin(rotation.x);
			const F cosRet = SupportingMath::Cos(rotation.x);


			ret.At(1, 1) = cosRet;
			ret.At(2, 1) = -sinRet;
			ret.At(1, 2) = sinRet;
			ret.At(2, 2) = cosRet;

			return ret;
		}

		template<uint8_t Alignment = 4> requires(Alignment >= 4)
		LAB_constexpr Matrix<F, 4, 4, Alignment> GetRotationYMatrix() const {
			Matrix<F, 4, 4> ret{ F(0) };
			ret.At(1, 1) = F(1);
			ret.At(3, 3) = 1;

			const F sinRet = SupportingMath::Sin(rotation.y);
			const F cosRet = SupportingMath::Cos(rotation.y);

			ret.At(0, 0) = cosRet;
			ret.At(2, 0) = sinRet;
			ret.At(0, 2) = -sinRet;
			ret.At(2, 2) = cosRet;

			return ret;
		}

		template<uint8_t Alignment = 4> requires(Alignment >= 4)
		LAB_constexpr Matrix<F, 4, 4, Alignment> GetRotationZMatrix() const {
			Matrix<F, 4, 4> ret{ F(0) };

			ret.At(2, 2) = F(1);
			ret.At(3, 3) = 1;

			const F sinRet = SupportingMath::Sin(rotation.z);
			const F cosRet = SupportingMath::Cos(rotation.z);

			ret.At(0, 0) = cosRet;
			ret.At(1, 0) = -sinRet;
			ret.At(0, 1) = sinRet;
			ret.At(1, 1) = cosRet;

			return ret;
		}

		template<uint8_t Alignment = 4> requires(Alignment >= 4)
		LAB_constexpr Matrix<F, 4, 4, Alignment> GetScaleMatrix() const {
			Matrix<F, 4, 4, Alignment> ret;
			ret.At(0, 0) = scale.x;
			ret.At(1, 1) = scale.y;
			ret.At(2, 2) = scale.z;
			ret.At(3, 3) = 1.f;

			ret.At(0, 1) = 0.f;
			ret.At(0, 2) = 0.f;
			ret.At(0, 3) = 0.f;

			ret.At(1, 0) = 0.f;
			ret.At(1, 2) = 0.f;
			ret.At(1, 3) = 0.f;
			
			ret.At(2, 0) = 0.f;
			ret.At(2, 1) = 0.f;
			ret.At(2, 3) = 0.f;

			ret.At(3, 0) = 0.f;
			ret.At(3, 1) = 0.f;
			ret.At(3, 2) = 0.f;
			return ret;
		}
    
		template<uint8_t Alignment = 4> requires(Alignment >= 4)
		LAB_constexpr Matrix<F, 4, 4, Alignment> GetMatrix(){
			Matrix<F, 4, 4, Alignment> ret;

			const F cosZ = SupportingMath::Cos(rotation.z);
			const F sinZ = SupportingMath::Sin(rotation.z);
			const F cosY = SupportingMath::Cos(rotation.y);
			const F sinY = SupportingMath::Sin(rotation.y);
			const F cosX = SupportingMath::Cos(rotation.x);
			const F sinX = SupportingMath::Sin(rotation.x);

			ret.At(0, 0) = scale.x * (cosY * cosZ + sinY * sinX * sinZ);
			ret.At(0, 1) = scale.x * (cosX * sinZ);
			ret.At(0, 2) = scale.x * (cosY * sinX * sinZ - cosZ * sinY);
			ret.At(0, 3) = 0.f;

			ret.At(1, 0) = scale.y * (cosZ * sinY * sinX - cosY * sinZ);
			ret.At(1, 1) = scale.y * (cosX * cosZ);
			ret.At(1, 2) = scale.y * (cosY * cosZ * sinX + sinY * sinZ);
			ret.At(1, 3) = 0.f;

			ret.At(2, 0) = scale.z * (cosX * sinY);
			ret.At(2, 1) = scale.z * (-sinX);
			ret.At(2, 2) = scale.z * (cosY * cosX);
			ret.At(2, 3) = 0.0f;

			ret.At(3, 0) = translation.x;
			ret.At(3, 1) = translation.y;
			ret.At(3, 2) = translation.z;
			ret.At(3, 3) = 1.0f;

			return ret;
		}
	
	};


}


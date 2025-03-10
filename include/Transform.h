#pragma once
#include "Vector.h"
#include "Matrix.h"

#include <concepts>


namespace Linear_Algebra {

    template<std::floating_point F, std::uint8_t Dimensions> requires((Dimensions > 1) && (Dimensions <= 4))
    struct Transform{};

    template<std::floating_point F>
    struct Transform<F, 2> {
        Vector<F, 2> translation;
        Vector<F, 2> scale;
        F rotation;

		//Transform() {}
        constexpr Transform() : translation{F(0)}, scale{F(1)}, rotation{F(0)} {}
        constexpr Transform(Vector<F, 2> const translation) : translation{translation}, scale{F(1)}, rotation{F(0)}{}
        constexpr Transform(Vector<F, 2> const translation, Vector<F, 2> const scale) : translation{translation}, scale{scale}, rotation{F(0)}{}
        constexpr Transform(Vector<F, 2> const translation, Vector<F, 2> const scale, F const rotation) : translation{translation}, scale{scale}, rotation{rotation}{}

        
        template<uint8_t Alignment = 12> requires(Alignment >= 12)
        constexpr Matrix<F, 3, 3, Alignment> ToMatrix() const {
            //if Y up and Z forward

            //this is temp just for debugging the templates
            Matrix<F, 3, 3, Alignment> ret;
            ret.At(0, 0) = translation.x;
            ret.At(0, 1) = translation.y;
            ret.At(0, 2) = 0.f;

            ret.At(1, 0) = scale.x;
            ret.At(1, 1) = scale.y;
            ret.At(1, 2) = 0.f;

            ret.At(2, 0) = rotation;
            ret.At(2, 1) = 0.f;
            ret.At(2, 2) = 1.f;

            return ret;
        }
        
    };

    template<std::floating_point F>
    struct Transform<F, 3> {
        Vector<F, 3> translation;
        Vector<F, 3> scale;
        Vector<F, 3> rotation;

		template<uint8_t Alignment = 16> requires(Alignment >= 16 && (Alignment % sizeof(float) == 0))
		constexpr Matrix<float, 4, 4, Alignment> GetRotationXMatrix() {
			Matrix<float, 4, 4> ret{ 0.f };

			ret.At(0, 0) = 1;
			ret.At(3, 3) = 1;

			const F sinRet = SupportingMath::Sin(rotation.x);
			const F cosRet = SupportingMath::Cos(rotation.x);


			ret.At(1, 1) = cosRet;
			ret.At(2, 1) = -sinRet;
			ret.At(1, 2) = sinRet;
			ret.At(2, 2) = cosRet;

			return ret;
		}

		template<uint8_t Alignment = 16> requires(Alignment >= 16 && (Alignment % sizeof(float) == 0))
		constexpr Matrix<float, 4, 4, Alignment> GetRotationYMatrix() {
			Matrix<float, 4, 4> ret{ 0.f };
			ret.At(1, 1) = 1;
			ret.At(3, 3) = 1;

			const F sinRet = SupportingMath::Sin(rotation.y);
			const F cosRet = SupportingMath::Cos(rotation.y);

			ret.At(0, 0) = cosRet;
			ret.At(2, 0) = sinRet;
			ret.At(0, 2) = -sinRet;
			ret.At(2, 2) = cosRet;

			return ret;
		}

		template<uint8_t Alignment = 16> requires(Alignment >= 16 && (Alignment % sizeof(float) == 0))
		constexpr Matrix<float, 4, 4, Alignment> GetRotationZMatrix() {
			Matrix<float, 4, 4> ret{ 0.f };
			ret.At(2, 2) = 1;
			ret.At(3, 3) = 1;

			const F sinRet = SupportingMath::Sin(rotation.z);
			const F cosRet = SupportingMath::Cos(rotation.z);

			ret.At(0, 0) = cosRet;
			ret.At(1, 0) = -sinRet;
			ret.At(0, 1) = sinRet;
			ret.At(1, 1) = cosRet;
		}

		template<uint8_t Alignment = 16> requires(Alignment >= 16 && (Alignment % sizeof(float) == 0))
		constexpr Matrix<float, 4, 4> GetScaleMatrix() {
			Matrix<float, 4, 4, Alignment> ret;
			ret.At(0, 0) = scale.x;
			ret.At(1, 1) = scale.y;
			ret.At(2, 2) = scale.z;
			return ret;
		}
    };


}


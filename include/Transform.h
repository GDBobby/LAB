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

        //id like to allow it to not be given default construction values, but it looks like its required with constexpr?
        constexpr Transform() : translation{F(0)}, scale{F(1)}, rotation{F(0)} {}
        constexpr Transform(Vector<F, 2> const translation) : translation{translation}, scale{F(1)}, rotation{F(0)}{}
        constexpr Transform(Vector<F, 2> const translation, Vector<F, 2> const scale) : translation{translation}, scale{scale}, rotation{F(0)}{}
        constexpr Transform(Vector<F, 2> const translation, Vector<F, 2> const scale, F const rotation) : translation{translation}, scale{scale}, rotation{}{}

        
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
    };


}
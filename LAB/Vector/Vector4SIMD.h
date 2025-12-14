#pragma once

#include "Vector4f.h"

#ifdef USING_SIMD

//need to come back and put constexpr branches into everything

namespace lab{
    struct VectorSIMD {
        union {
            Vector<float, 4> component;
            __m128 vec;
        };
        //simd cant be constexpr (currently)
        VectorSIMD() : vec{} {}
        VectorSIMD(__m128 const& vec) : vec{vec} {}
        LAB_constexpr VectorSIMD(const float x, const float y, const float z, const float w) : component{x, y, z, w} {}
        LAB_constexpr VectorSIMD(lab::Vector<float, 4> const& vec) : component{ vec } {}

        VectorSIMD(VectorSIMD const& other) : vec{ other.vec } {}//copy
        VectorSIMD(VectorSIMD&& other) noexcept : vec{ other.vec } {} //move
        LAB_constexpr VectorSIMD& operator=(VectorSIMD const& other){ //copy assignment
            if constexpr (std::is_constant_evaluated()) {
                component = other.component;
            }
            else {
                vec = other.vec;
            }
            return *this;
        }
        LAB_constexpr VectorSIMD& operator=(VectorSIMD&& other) { //move assignment
            if constexpr (std::is_constant_evaluated()) {
                component = other.component;
            }
            else {
                vec = other.vec;
            }
            return *this;
        }


        operator __m128() const {
            return vec;
        }
        VectorSIMD operator-() const{
            return VectorSIMD{
                _mm_xor_ps(vec, _mm_set_ps1(-0.f))
            };
        }
        bool operator==(VectorSIMD const other) const {
            return _mm_movemask_ps(_mm_cmpeq_ps(vec, other.vec)) == 0xF;
        }
        VectorSIMD operator+(VectorSIMD const other) const {
            return VectorSIMD(_mm_add_ps(vec, other.vec));
        }
        VectorSIMD& operator+=(VectorSIMD const other) {
            vec = _mm_add_ps(vec, other.vec);
            return *this;
        }

        VectorSIMD operator*(VectorSIMD const other) const {
            return VectorSIMD(_mm_mul_ps(vec, other.vec));
        }
        VectorSIMD& operator*=(VectorSIMD const other) {
            vec = _mm_mul_ps(vec, other.vec);
            return *this;
        }
        
        VectorSIMD operator*(float const multiplier) const {
            return VectorSIMD{_mm_mul_ps(vec, _mm_set_ps1(multiplier)) };
        }
        VectorSIMD& operator*=(float const multiplier) {
            vec = _mm_mul_ps(vec, _mm_set_ps1(multiplier));
            return *this;
        }

        VectorSIMD& operator-=(VectorSIMD const other) {
            vec = _mm_sub_ps(vec, other.vec);
            return *this;
        }
        VectorSIMD operator-(VectorSIMD const other) const {
            return VectorSIMD{_mm_sub_ps(vec, other.vec)};
        }

        VectorSIMD& operator/=(float const divisor) {
#if LAB_DEBUGGING_FLOAT_ANOMALIES
            //handle divider == 0
#endif
            vec = _mm_div_ps(vec, _mm_set_ps1(divisor));
            return *this;
        }
        VectorSIMD operator/(float const divisor) const {
#if LAB_DEBUGGING_FLOAT_ANOMALIES
            //handle divider == 0
#endif
            return VectorSIMD{_mm_div_ps(vec, _mm_set_ps1(divisor))};
        }

        LAB_constexpr float& operator[](uint8_t const row) {
            if (row == 0) {
                return component.x;
            }
            else if (row == 1) {
                return component.y;
            }
            else if (row == 2) {
                return component.z;
            }
            else if (row == 3) {
                return component.w;
            }
            LAB_UNREACHABLE;
        }

        LAB_constexpr float operator[](uint8_t const row) const {
            if (row == 0) {
                return component.x;
            }
            else if (row == 1) {
                return component.y;
            }
            else if (row == 2) {
                return component.z;
            }
            else if (row == 3) {
                return component.w;
            }
            LAB_UNREACHABLE;
        }

        float SquaredMagnitude() const {
            return component.x * component.x + component.y * component.y + component.z * component.z + component.w * component.w;
        }        
        float Magnitude() const {
            return Sqrt(SquaredMagnitude());
        }
        VectorSIMD& Normalize() {
            const float invMag = InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        VectorSIMD Normalized() const {
            const auto invMag = InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }
        float Dot(VectorSIMD const other) const {
            return component.x * other.component.x + component.y * other.component.y + component.z * other.component.z + component.w * other.component.w;
        }
    };
}
#endif
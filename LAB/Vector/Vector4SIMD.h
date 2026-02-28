#pragma once

#include "Vector4f.h"

#include <memory>

#ifdef USING_SIMD

//need to come back and put constexpr branches into everything

namespace lab{
    struct VectorSIMD {
        union {
            Vector<float, 4> component;
            __m128 vec;
        };
        //simd cant be constexpr (currently)
        LAB_constexpr VectorSIMD() : vec{} {}
        VectorSIMD(__m128 const& vec) : vec{vec} {}
        LAB_constexpr VectorSIMD(const float x, const float y, const float z, const float w) : component{x, y, z, w} {}
        LAB_constexpr VectorSIMD(Vector<float, 4> const& vec) : component{ vec } {}

        LAB_constexpr VectorSIMD(VectorSIMD const& other) noexcept {
            if consteval{
                std::construct_at(&component, other.component);
            }
            else{
                vec = other.vec;
            }
        }
        LAB_constexpr VectorSIMD(VectorSIMD&& other) noexcept {
            if consteval{
                std::construct_at(&component, other.component);
            }
            else{
                vec = other.vec;
            }
        }
        LAB_constexpr VectorSIMD& operator=(VectorSIMD const& other){ //copy assignment
            if consteval {
                component = other.component;
            }
            else {
                vec = other.vec;
            }
            return *this;
        }
        LAB_constexpr VectorSIMD& operator=(VectorSIMD&& other) { //move assignment
            if consteval {
                component = other.component;
            }
            else {
                vec = other.vec;
            }
            return *this;
        }


        LAB_constexpr operator __m128() const {
            return vec;
        }
        LAB_constexpr operator Vector<float, 4>() const{
            return component;
        }

        VectorSIMD operator-() const{
            if consteval{
                return VectorSIMD{
                    -component
                };
            }
            else{
                return VectorSIMD{
                    _mm_xor_ps(vec, _mm_set_ps1(-0.f)) //this doesnt seem correct
                };
            }
        }
        LAB_constexpr bool operator==(VectorSIMD const other) const {
            if consteval{
                return component == other.component;
            }
            else{
                return _mm_movemask_ps(_mm_cmpeq_ps(vec, other.vec)) == 0xF;
            }
        }
        LAB_constexpr VectorSIMD operator+(VectorSIMD const other) const {
            if consteval{
                return VectorSIMD{
                    component + other.component
                };
            }
            else{
                return VectorSIMD(_mm_add_ps(vec, other.vec));
            }
        }
        LAB_constexpr VectorSIMD& operator+=(VectorSIMD const other) {
            if consteval{
                component += other.component;
            }
            else{
                vec = _mm_add_ps(vec, other.vec);
            }
            return *this;
        }

        LAB_constexpr VectorSIMD operator*(VectorSIMD const other) const {
            if consteval{
                return VectorSIMD{component * other.component};
            }
            else{
                return VectorSIMD(_mm_mul_ps(vec, other.vec));
            }
        }
        LAB_constexpr VectorSIMD& operator*=(VectorSIMD const other) {
            if consteval{
                component *= other.component;
            }
            else{
                vec = _mm_mul_ps(vec, other.vec);
            }
            return *this;
        }
        
        LAB_constexpr VectorSIMD operator*(float const multiplier) const {
            if consteval{
                return VectorSIMD{component * multiplier};
            }
            else{
                return VectorSIMD{_mm_mul_ps(vec, _mm_set_ps1(multiplier)) };
            }
        }

        LAB_constexpr VectorSIMD& operator*=(float const multiplier) {
            if consteval{
                component *= multiplier;
            }
            else{
                vec = _mm_mul_ps(vec, _mm_set_ps1(multiplier));
            }
            return *this;
        }

        LAB_constexpr VectorSIMD& operator-=(VectorSIMD const other) {
            if consteval{
                component -= other.component;
            }
            else{
                vec = _mm_sub_ps(vec, other.vec);
            }
            return *this;
        }
        LAB_constexpr VectorSIMD operator-(VectorSIMD const other) const {
            if consteval{
                return VectorSIMD{component - other.component};
            }
            else{
                return VectorSIMD{_mm_sub_ps(vec, other.vec)};
            }
        }

        LAB_constexpr VectorSIMD& operator/=(float const divisor) {
#if LAB_DEBUGGING_FLOAT_ANOMALIES
            //handle divider == 0
#endif
            if consteval{
                component /= divisor;
            }
            else{
                vec = _mm_div_ps(vec, _mm_set_ps1(divisor));
            }
            return *this;
        }
        LAB_constexpr VectorSIMD operator/(float const divisor) const {
#if LAB_DEBUGGING_FLOAT_ANOMALIES
            //handle divider == 0
#endif
            if consteval{
                return VectorSIMD{component / divisor};
            }
            else{
                return VectorSIMD{_mm_div_ps(vec, _mm_set_ps1(divisor))};
            }
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

        LAB_constexpr float SquaredMagnitude() const {
            return component.x * component.x + component.y * component.y + component.z * component.z + component.w * component.w;
        }        
        LAB_constexpr float Magnitude() const {
            return Sqrt(SquaredMagnitude());
        }
        LAB_constexpr VectorSIMD& Normalize() {
            const float invMag = InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        LAB_constexpr VectorSIMD Normalized() const {
            const auto invMag = InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }
        LAB_constexpr float Dot(VectorSIMD const other) const {
            return component.x * other.component.x + component.y * other.component.y + component.z * other.component.z + component.w * other.component.w;
        }
    };
}
#endif
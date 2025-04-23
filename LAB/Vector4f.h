#pragma once
#include "VectorTemplate.h"

#if 1//AVX
#include <immintrin.h>
#elif SSE
#include <xmmintrin.h>
#endif

namespace lab{
    template<>
    struct Vector<float, 4> {
        union{
            struct {
                float x;
                float y;
                float z;
                float w;
            };
            __m128 vec;
        };

        LAB_constexpr Vector() {}
        LAB_constexpr Vector(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w } {}
        LAB_constexpr Vector(float all) : x{ all }, y{ all }, z{ all }, w{ all } {}
        LAB_constexpr Vector(Vector<float, 2> const& other) : x{ other.x }, y{ other.y }, z{ float(0)}, w{float(0)} {}
        LAB_constexpr Vector(Vector<float, 3> const& other) : x{ other.x }, y{ other.y }, z{ other.z }, w{ float(0) } {}
        LAB_constexpr Vector(Vector const& other) : x{ other.x }, y{ other.y }, z{ other.z }, w{ other.w } {}
        
        //this cant be constexpr
        Vector(__m128 const& vec) : vec{vec} {}

        LAB_constexpr Vector& operator=(Vector const& other){
            if constexpr (std::is_constant_evaluated()){
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
            }
            else{
                vec = other.vec;
            }
            return *this;
        }

        LAB_constexpr float& operator[](uint8_t const row) {
            if (row == 0) {
                return x;
            }
            else if (row == 1) {
                return y;
            }
            else if (row == 2) {
                return z;
            }
            else if (row == 3) {
                return w;
            }
            assert(row < 4);
            return x;
        }
        LAB_constexpr float operator[](uint8_t const row) const {
            if (row == 0) {
                return x;
            }
            else if (row == 1) {
                return y;
            }
            else if (row == 2) {
                return z;
            }
            else if (row == 3) {
                return w;
            }
            assert(row < 4);
            return x;
        }

        template<uint8_t DimensionsOther>
        LAB_constexpr Vector& operator=(Vector<float, DimensionsOther> const& other) {
            if constexpr (DimensionsOther == 2) {
                x = other.x;
                y = other.y;
            }
            else if constexpr (DimensionsOther == 3) {
                x = other.x;
                y = other.y;
                z = other.z;
            }
            else if constexpr (DimensionsOther == 4) {
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
            }
            return *this;
        }

        LAB_constexpr bool operator==(Vector const other) const {
            return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
        }
        LAB_constexpr void operator+=(Vector const other) {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
        }
        LAB_constexpr Vector operator+(Vector const other) const {
            return Vector{
                x + other.x,
                y + other.y,
                z + other.z,
                w + other.w
            };
        }
        LAB_constexpr Vector operator*(Vector const other) const {
            return {
                x * other.x,
                y * other.y,
                z * other.z,
                w * other.w
            };
        }

        LAB_constexpr void operator-=(Vector const other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
        }
        LAB_constexpr Vector operator-(Vector const other) const {
            return Vector{
                x - other.x,
                y - other.y,
                z - other.z,
                w - other.w
            };
        }
        LAB_constexpr void operator*=(float const multiplier) {
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
            w *= multiplier;
        }
        LAB_constexpr Vector operator*(float const multiplier) const {
            return Vector{
                x * multiplier,
                y * multiplier,
                z * multiplier,
                w * multiplier
            };
        }
        LAB_constexpr void operator/=(float const divisor) {
            x /= divisor;
            y /= divisor;
            z /= divisor;
            w /= divisor;
        }
        LAB_constexpr Vector operator/(float const divisor) const {
            return Vector{
                x / divisor,
                y / divisor,
                z / divisor,
                w / divisor
            };
        }
        LAB_constexpr Vector& operator*=(Vector const other){
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        }

        LAB_constexpr float SquaredMagnitude() const {
            return x * x + y * y + z * z + w * w;
        }

        LAB_constexpr float Magnitude() const {
            return SupportingMath::Sqrt(SquaredMagnitude());
        }

        LAB_constexpr Vector& Normalize() {
            const float invMag = SupportingMath::InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        LAB_constexpr Vector Normalized() const{
            const auto invMag = SupportingMath::InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }

        LAB_constexpr float DotProduct(Vector const& other) const {
            if(std::is_constant_evaluated()){
                return x * other.x + y * other.y + z * other.z + w * other.w;
            }
            else{
                Vector temp{_mm_mul_ps(vec, other.vec)};
                return temp.x + temp.y + temp.z + temp.w;
            }
        }

        LAB_constexpr static Vector Forward() {
            return { float(1), float(0), float(0), float(0) };
        }
        LAB_constexpr static Vector Up() {
            return { float(0), float(0), float(1), float(0) };
        }
        LAB_constexpr static Vector Right() {
            return { float(0), float(1), float(0), float(0) };
        }
        LAB_constexpr static Vector Ahead() {
            return { float(0), float(0), float(0), float(1) };
        }
    };


    
}

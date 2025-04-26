#pragma once
#include "VectorTemplate.h"

#if defined(USING_SSE) || (USING_AVX2)
#define USING_SIMD
#include <immintrin.h>
#endif

namespace lab{
    template<>
    struct Vector<float, 4> {
#ifdef USING_SIMD
        union{
            struct {
                float x;
                float y;
                float z;
                float w;
            };
            __m128 vec;
        };
#else
        float x;
        float y;
        float z;
        float w;
#endif

        LAB_constexpr Vector() {}
        LAB_constexpr Vector(float const x, float const y, float const z, float const w) : x{ x }, y{ y }, z{ z }, w{ w } {}
        explicit LAB_constexpr Vector(float const all) : x{ all }, y{ all }, z{ all }, w{ all } {}

        //constructing piece wise with vec2
        LAB_constexpr Vector(Vector<float, 2> const vec1, Vector<float, 2> const vec2) : x{vec1.x}, y{vec1.y}, z{vec2.x}, w{vec2.y} {}
        LAB_constexpr Vector(Vector<float, 2> const vec, float const z, float const w) : x{vec.x}, y{vec.y}, z{z}, w{w} {}
        LAB_constexpr Vector(float const x, Vector<float, 2> const vec, float const w) : x{x}, y{vec.x}, z{vec.y}, w{w} {}
        LAB_constexpr Vector(float const x, float const y, Vector<float, 2> const vec) : x{x}, y{y}, z{vec.x}, w{vec.y} {}
        //constructing piecewise with vec3
        LAB_constexpr Vector(Vector<float, 3> const vec, float const w) : x{vec.x}, y{vec.y}, z{vec.z}, w{w} {}
        LAB_constexpr Vector(float const x, Vector<float, 3> const vec) : x{x}, y{vec.x}, z{vec.y}, w{vec.z} {}
        //copy constructors, including vec2 and vec3
        //do i need to make these explicit?
        LAB_constexpr Vector(Vector<float, 2> const& other) : x{ other.x }, y{ other.y }, z{ float(0)}, w{float(0)} {}
        LAB_constexpr Vector(Vector<float, 3> const& other) : x{ other.x }, y{ other.y }, z{ other.z }, w{ float(0) } {}
        LAB_constexpr Vector(Vector const& other) : x{ other.x }, y{ other.y }, z{ other.z }, w{ other.w } {}
        
        //simd cant be constexpr (currently)
#ifdef USING_SIMD
        Vector(__m128 const& vec) : vec{vec} {}
#endif

        LAB_constexpr Vector& operator=(Vector const& other){
#ifdef USING_SIMD
            if constexpr (std::is_constant_evaluated()){
#endif
                x = other.x;
                y = other.y;
                z = other.z;
                w = other.w;
#ifdef USING_SIMD
            }
            else{
                //i need to benchmark if this is actually better
                vec = other.vec;
            }
#endif
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
            LAB_UNREACHABLE;
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
            LAB_UNREACHABLE;
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
#ifdef USING_SIMD

#endif
            return Vector{
                x - other.x,
                y - other.y,
                z - other.z,
                w - other.w
            };
        }
        LAB_constexpr Vector& operator*=(float const multiplier) {
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
            w *= multiplier;
            return *this;
        }
        LAB_constexpr Vector operator*(float const multiplier) const {
            return Vector{
                x * multiplier,
                y * multiplier,
                z * multiplier,
                w * multiplier
            };
        }
        LAB_constexpr Vector& operator/=(float const divisor) {
            x /= divisor;
            y /= divisor;
            z /= divisor;
            w /= divisor;
            return *this;
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
            return Sqrt(SquaredMagnitude());
        }

        LAB_constexpr Vector& Normalize() {
            const float invMag = InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        LAB_constexpr Vector Normalized() const{
            const auto invMag = InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }

        LAB_constexpr float Dot(Vector const other) const {
#ifdef USING_SIMD
            if constexpr (std::is_constant_evaluated()){
#endif
                return x * other.x + y * other.y + z * other.z + w * other.w;
#ifdef USING_SIMD
            }
            else{
                Vector temp{_mm_mul_ps(vec, other.vec)};
                return temp.x + temp.y + temp.z + temp.w;
            }
#endif
        }
    };
}

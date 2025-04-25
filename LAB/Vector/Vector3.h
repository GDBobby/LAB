#pragma once
#include "VectorTemplate.h"

namespace lab{
    template<std::floating_point F>
    struct Vector<F, 3> {
        F x;
        F y;
        F z;
        LAB_constexpr Vector() {}
        LAB_constexpr Vector(F const x, F const y, F const z) : x{ x }, y{ y }, z{ z } {}
        explicit LAB_constexpr Vector(F const all) : x{ all }, y{ all }, z{ all } {}
        //constructing piecewise with vec2
        LAB_constexpr Vector(Vector<F, 2> const vec, F const z) : x{vec.x}, y{vec.y}, z{z}{}
        LAB_constexpr Vector(F const x, Vector<F, 2> const vec) : x{x}, y{vec.x}, z{vec.y}{}
        //constructing with vec4, dropping vec4.w
        explicit LAB_constexpr Vector(Vector<F, 4> const vec) : x{vec.x}, y{vec.y}, z{vec.z}{}
        //copy constructors, including vec2
        LAB_constexpr Vector(Vector<F, 2> const& other) : x{ other.x }, y{ other.y }, z{F(0)} {}
        LAB_constexpr Vector(Vector const& other) : x{ other.x }, y{ other.y }, z{ other.z } {}
        LAB_constexpr Vector& operator=(Vector const& other){
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        LAB_constexpr F& operator[](uint8_t const row) {
            if (row == 0) {
                return x;
            }
            else if (row == 1) {
                return y;
            }
            else if (row == 2) {
                return z;
            }
            LAB_UNREACHABLE;
        }
        LAB_constexpr F operator[](uint8_t const row) const {
            if (row == 0) {
                return x;
            }
            else if (row == 1) {
                return y;
            }
            else if (row == 2) {
                return z;
            }
            LAB_UNREACHABLE;
        }
        template<uint8_t DimensionsOther> 
        LAB_constexpr Vector& operator=(Vector<F, DimensionsOther> const& other) {
            if constexpr (DimensionsOther == 2) {
                x = other.x;
                y = other.y;
            }
            else if constexpr (DimensionsOther >= 3) {
                x = other.x;
                y = other.y;
                z = other.z;
            }
            return *this;
        }
        LAB_constexpr bool operator==(Vector const other) const {
            return (x == other.x) && (y == other.y) && (z == other.z);
        }
        LAB_constexpr void operator+=(Vector const other) {
            x += other.x;
            y += other.y;
            z += other.z;
        }
        LAB_constexpr Vector operator+(Vector const other) const {
            return Vector{
                x + other.x,
                y + other.y,
                z + other.z
            };
        }
        LAB_constexpr void operator-=(Vector const other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }
        LAB_constexpr Vector operator-(Vector const other) const {
            return Vector{
                x - other.x,
                y - other.y,
                z - other.z
            };
        }
        LAB_constexpr void operator*=(F const multiplier) {
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
        }
        LAB_constexpr Vector operator*(F const multiplier) const {
            return Vector{
                x * multiplier,
                y * multiplier,
                z * multiplier
            };
        }
        LAB_constexpr Vector operator*(Vector const other) const {
            return Vector{
                x * other.x,
                y * other.y,
                z * other.z
            };
        }
        LAB_constexpr Vector& operator*=(Vector const other){
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }
        LAB_constexpr void operator/=(F const divisor) {
            x /= divisor;
            y /= divisor;
            z /= divisor;
        }
        LAB_constexpr Vector operator/(F const divisor) const {
            return Vector{
                x / divisor,
                y / divisor,
                z / divisor
            };
        }
        
        LAB_constexpr F SquaredMagnitude() const {
            return x * x + y * y + z * z;
        }

        LAB_constexpr F Magnitude() const {
            return Sqrt(SquaredMagnitude());
        }

        LAB_constexpr Vector& Normalize() {
            const F invMag = InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        LAB_constexpr Vector Normalized() const {
            const auto invMag = InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }
        LAB_constexpr F Dot(Vector const other) const {
            return x * other.x + y * other.y + z * other.z;
        }
        LAB_constexpr Vector Cross(Vector const other) const {		
            return Vector{
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            };
        }
    };

    template<std::floating_point F>
    Vector<F, 3> operator/(F const f, Vector<F, 3> const vec){
        return {
            f / vec.x,
            f / vec.y,
            f / vec.z
        };
    }
    
    template<std::floating_point F>
    Vector<F, 3> operator-(F const f, Vector<F, 3> const vec){
        return {
            f - vec.x,
            f - vec.y,
            f - vec.z
        };
    }
}
#pragma once
#include "VectorTemplate.h"

namespace lab{
    template<std::floating_point F>
    struct Vector<F, 2> {
        F x;
        F y;
        LAB_constexpr Vector() {}
        LAB_constexpr Vector(F const x, F const y) : x{ x }, y{ y } {}
        explicit LAB_constexpr Vector(F const all) : x{ all }, y{ all } {}

        //copy constructors, including vec3 and vec4, z and w will be dropped
        LAB_constexpr Vector(Vector<F, 3> const& vec) : x{vec.x}, y{vec.y} {
            /* i been wanting this but i dont know if its a good idea
            if constexpr(CoordinateSystem::up == CoordinateSystem::XNeg){
                x = vec.y;
                y = vec.z;
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::XPos){
                x = vec.y;
                y = vec.z;
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::YNeg){
                x = vec.x;
                y = vec.z;
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::YPos){
                x = vec.x;
                y = vec.z;
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::ZNeg){
                x = vec.x;
                y = vec.y;
            }
            //itll never not be one of these or its an error. id make it explicitly else if but i dont feel like dealing with the warning
            else {//if constexpr(CoordinateSystem::up == CoordinateSystem::ZNeg){
                x = vec.x;
                y = vec.y;
            }
            */
        }
        LAB_constexpr Vector(Vector<F, 4> const& vec) : x{vec.x}, y{vec.y} {}
        LAB_constexpr Vector(Vector const& other) : x{ other.x }, y{ other.y } {}

        LAB_constexpr Vector& operator=(Vector const& other){
            x = other.x;
            y = other.y;
            return *this;
        }

        LAB_constexpr F& operator[](uint8_t const row) {
            if (row == 0) {
                return x;
            }
            else if (row == 1) {
                return y;
            }
            assert(row < 2);
            return x;
            
        }
        LAB_constexpr F operator[](uint8_t const row) const {
            if (row == 0) {
                return x;
            }
            else if (row == 1) {
                return y;
            }
            assert(row < 2);
            return x;
            
        }

        template<uint8_t DimensionsOther>
        LAB_constexpr Vector& operator=(Vector<F, DimensionsOther> const& other) {
            x = other.x;
            y = other.y;
            return *this;
        }

        LAB_constexpr bool operator==(Vector const other) const {
            return (x == other.x) && (y == other.y);
        }
        LAB_constexpr void operator+=(Vector const other) {
            x += other.x;
            y += other.y;
        }
        LAB_constexpr Vector operator+(Vector const other) const {
            return {
                x + other.x,
                y + other.y
            };
        }
        LAB_constexpr void operator-=(Vector const other) {
            x -= other.x;
            y -= other.y;
        }
        LAB_constexpr Vector operator-(Vector const other) const {
            return {
                x - other.x,
                y - other.y
            };
        }
        LAB_constexpr void operator*=(F const multiplier) {
            x *= multiplier;
            y *= multiplier;
        }
        LAB_constexpr Vector operator*(F const multiplier) const {
            return {
                x * multiplier,
                y * multiplier
            };
        }
        LAB_constexpr Vector operator*(Vector const other) const {
            return {
                x * other.x,
                y * other.y
            };
        }
        LAB_constexpr void operator/=(F const divisor) {
            x /= divisor;
            y /= divisor;
        }
        LAB_constexpr Vector operator/(F const divisor) const {
            return Vector{
                x / divisor,
                y / divisor,
            };
        }
        
        LAB_constexpr F SquaredMagnitude() const {
            return x * x + y * y;
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

        LAB_constexpr F Dot(Vector const& other) const {
            return x * other.x + y * other.y;
        }
    };

    template<std::floating_point F>
    Vector<F, 2> operator/(F const f, Vector<F, 2> const vec){
        return {
            f / vec.x,
            f / vec.y
        };
    }
    
    template<std::floating_point F>
    Vector<F, 2> operator-(F const f, Vector<F, 2> const vec){
        return {
            f - vec.x,
            f - vec.y
        };
    }
} //namespace lab


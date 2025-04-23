#pragma once
#include "VectorTemplate.h"

namespace lab{
    template<std::floating_point F>
    struct Vector<F, 2> {
        F x;
        F y;
        LAB_constexpr Vector() {}
        LAB_constexpr Vector(F const x, F const y) : x{ x }, y{ y } {}
        LAB_constexpr Vector(F const all) : x{ all }, y{ all } {}

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
            return SupportingMath::Sqrt(SquaredMagnitude());
        }

        LAB_constexpr Vector& Normalize() {
            const F invMag = SupportingMath::InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        LAB_constexpr Vector Normalized() const {
            const auto invMag = SupportingMath::InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }

        LAB_constexpr F DotProduct(Vector const& other) const {
            return x * other.x + y * other.y;
        }

        LAB_constexpr static Vector Forward() {
            if constexpr (CoordinateSystem::forward == CoordinateSystem::XPos){
                return { F(1), F(0) };
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::XNeg){
                return {F(-1), F(0)};
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::YPos){
                return {F(0), F(1)};
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::YNeg){
                return {F(0), F(-1)};
            }
            else{
                static_assert(false && "idk how to rectify Z forward/back/up/down in 2 dimensions yet. I'm aware 3D environments will frequently use 2 dimensional vectors");
            }
        }
        LAB_constexpr static Vector Up() {
            //switch LAB_constexpr where pls
            //i read that i could potentially expect it to look the same as if constexpr, but it's not guaranteed, and I'm relying on compiler optimization

            if constexpr (CoordinateSystem::up == CoordinateSystem::YPos){
                return { F(0), F(1) };
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::YNeg){
                return {F(0), F(-1)};
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::XPos){
                return {F(1), F(0)};
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::XNeg){
                return {F(-1), F(0)};
            }
            else{
                static_assert(false && "idk how to rectify Z forward/back/up/down in 2 dimensions yet. I'm aware 3D environments will frequently use 2 dimensional vectors");
            }
        }
    };
} //namespace lab


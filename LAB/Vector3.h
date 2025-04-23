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
        LAB_constexpr Vector(F const all) : x{ all }, y{ all }, z{ all } {}

        LAB_constexpr Vector(Vector<F, 2> const& other) : x{ other.x }, y{ other.y } {}
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
            assert(row < 3);
            return x;
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
            assert(row < 3);
            return x;
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
        LAB_constexpr void operator +=(Vector const other) {
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
            return {
                x * other.x,
                y * other.y,
                z * other.z
            };
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
        LAB_constexpr Vector& operator*=(Vector const other){
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
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

        LAB_constexpr F Dot(Vector const& other) const {
            return x * other.x + y * other.y + z * other.z;
        }
        LAB_constexpr Vector Cross(Vector const& other) const {		
            return Vector<F, 3>{
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            };
        }

        LAB_constexpr static Vector Forward() {
            if constexpr(CoordinateSystem::forward == CoordinateSystem::XNeg){
                return { F(-1), F(0), F(0) };
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::XPos){
                return { F(-1), F(0), F(0) };
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::YNeg){
                return { F(0), F(-1), F(0) };
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::YPos){
                return { F(0), F(1), F(0) };
            }
            else if constexpr(CoordinateSystem::forward == CoordinateSystem::ZNeg){
                return { F(0), F(0), F(-1) };
            }
            //itll never not be one of these or its an error
            else {//if constexpr(CoordinateSystem::forward == CoordinateSystem::ZNeg){
                return { F(0), F(0), F(1) };
            }
        }
        LAB_constexpr static Vector Up() {
            if constexpr(CoordinateSystem::up == CoordinateSystem::XNeg){
                return { F(-1), F(0), F(0) };
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::XPos){
                return { F(-1), F(0), F(0) };
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::YNeg){
                return { F(0), F(-1), F(0) };
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::YPos){
                return { F(0), F(1), F(0) };
            }
            else if constexpr(CoordinateSystem::up == CoordinateSystem::ZNeg){
                return { F(0), F(0), F(-1) };
            }
            //itll never not be one of these or its an error
            else {//if constexpr(CoordinateSystem::up == CoordinateSystem::ZNeg){
                return { F(0), F(0), F(1) };
            }
        }
        LAB_constexpr static Vector Right() {
            if constexpr(CoordinateSystem::right == CoordinateSystem::XNeg){
                return { F(-1), F(0), F(0) };
            }
            else if constexpr(CoordinateSystem::right == CoordinateSystem::XPos){
                return { F(-1), F(0), F(0) };
            }
            else if constexpr(CoordinateSystem::right == CoordinateSystem::YNeg){
                return { F(0), F(-1), F(0) };
            }
            else if constexpr(CoordinateSystem::right == CoordinateSystem::YPos){
                return { F(0), F(1), F(0) };
            }
            else if constexpr(CoordinateSystem::right == CoordinateSystem::ZNeg){
                return { F(0), F(0), F(-1) };
            }
            //itll never not be one of these or its an error
            else {//if constexpr(CoordinateSystem::right == CoordinateSystem::ZNeg){
                return { F(0), F(0), F(1) };
            }
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
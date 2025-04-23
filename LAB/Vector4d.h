#pragma once
#include "VectorTemplate.h"

namespace lab{
    template<>
    struct Vector<double, 4> {
        double x;
        double y;
        double z;
        double w;

        LAB_constexpr Vector() {}
        LAB_constexpr Vector(double x, double y, double z, double w) : x{ x }, y{ y }, z{ z }, w{ w } {}
        LAB_constexpr Vector(double all) : x{ all }, y{ all }, z{ all }, w{ all } {}
        LAB_constexpr Vector(Vector<double, 2> const& other) : x{ other.x }, y{ other.y }, z{ double(0)}, w{double(0)} {}
        LAB_constexpr Vector(Vector<double, 3> const& other) : x{ other.x }, y{ other.y }, z{ other.z }, w{ double(0) } {}
        LAB_constexpr Vector(Vector const& other) : x{ other.x }, y{ other.y }, z{ other.z }, w{ other.w } {}
        LAB_constexpr Vector& operator=(Vector const& other){
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        LAB_constexpr double& operator[](uint8_t const row) {
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
        LAB_constexpr double operator[](uint8_t const row) const {
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
        LAB_constexpr Vector& operator=(Vector<double, DimensionsOther> const& other) {
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
        LAB_constexpr void operator*=(double const multiplier) {
            x *= multiplier;
            y *= multiplier;
            z *= multiplier;
            w *= multiplier;
        }
        LAB_constexpr Vector operator*(double const multiplier) const {
            return Vector{
                x * multiplier,
                y * multiplier,
                z * multiplier,
                w * multiplier
            };
        }
        LAB_constexpr void operator/=(double const divisor) {
            x /= divisor;
            y /= divisor;
            z /= divisor;
            w /= divisor;
        }
        LAB_constexpr Vector operator/(double const divisor) const {
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
        LAB_constexpr Vector operator*(Vector const other) const {
            return Vector{
                x * other.x,
                y * other.y,
                z * other.z,
                w * other.w
            };
        }

        LAB_constexpr double SquaredMagnitude() const {
            return x * x + y * y + z * z + w * w;
        }

        LAB_constexpr double Magnitude() const {
            return Sqrt(SquaredMagnitude());
        }

        LAB_constexpr Vector& Normalize() {
            const double invMag = InverseSqrt(SquaredMagnitude());
            operator*=(invMag);
            return *this;
        }
        LAB_constexpr Vector Normalized() const{
            const auto invMag = InverseSqrt(SquaredMagnitude());
            return operator*(invMag);
        }

        LAB_constexpr double Dot(Vector const& other) const {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }

        LAB_constexpr static Vector Forward() {
            return { double(1), double(0), double(0), double(0) };
        }
        LAB_constexpr static Vector Up() {
            return { double(0), double(0), double(1), double(0) };
        }
        LAB_constexpr static Vector Right() {
            return { double(0), double(1), double(0), double(0) };
        }
        LAB_constexpr static Vector Ahead() {
            return { double(0), double(0), double(0), double(1) };
        }
    };

    Vector<double, 4> operator/(double const f, Vector<double, 4> const vec){
        return {
            f / vec.x,
            f / vec.y,
            f / vec.z,
            f / vec.w
        };
    }
    template<double>
    Vector<double, 4> operator-(double const f, Vector<double, 4> const vec){
        return {
            f - vec.x,
            f - vec.y,
            f - vec.z,
            f - vec.w
        };
    }
}

#pragma once

#include "SupportingMath.h"
#include "CoordinateSystems.h"
#include "Debugging.h"

#include <cstdint>
#include <concepts>

namespace LAB {
	//F short for floating point, can be double or float, Dimensions is short for dimensions
	template<std::floating_point F, uint8_t Dimensions> 
		requires((Dimensions > 1) && (Dimensions <= 4))
	struct Vector{};

	template<std::floating_point F>
	struct Vector<F, 2> {
		F x;
		F y;
		LAB_constexpr Vector() {}
		LAB_constexpr Vector(F const x, F const y) : x{ x }, y{ y } {}
		LAB_constexpr Vector(F const all) : x{ all }, y{ all } {}

		LAB_constexpr F& operator[](uint8_t const row) {
			if (row == 0) {
				return x;
			}
			else if (row == 1) {
				return y;
			}
		}
		LAB_constexpr F operator[](uint8_t const row) const {
			if (row == 0) {
				return x;
			}
			else if (row == 1) {
				return y;
			}
		}


		LAB_constexpr bool operator==(Vector const other) const {
			return (x == other.x) && (y == other.y);
		}
		LAB_constexpr void operator+= (Vector const other) {
			x += other.x;
			y += other.y;
		}
		LAB_constexpr Vector operator+(Vector const other) const {
			Vector ret = *this;
			ret += other;
			return ret;
		}
		LAB_constexpr void operator-=(Vector const other) {
			x -= other.x;
			y -= other.y;
		}
		LAB_constexpr Vector operator-(Vector const other) const {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		LAB_constexpr void operator *=(F const multiplier) {
			x *= multiplier;
			y *= multiplier;
		}
		LAB_constexpr Vector operator*(F const multiplier) const {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		LAB_constexpr void operator /=(F const divisor) {
			x /= divisor;
			y /= divisor;
		}
		LAB_constexpr Vector operator /(F const divisor) const {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}
		
		LAB_constexpr F SquaredMagnitude() const {
			return x * x + y * y;
		}

		LAB_constexpr F Magnitude() const {
			return SupportingMath::Sqrt(SquaredMagnitude());
		}

		LAB_constexpr void Normalize() {
			const auto mag = Magnitude();
			operator/=(mag);
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


	template<std::floating_point F>
	struct Vector<F, 3> {
		F x;
		F y;
		F z;
		LAB_constexpr Vector() {}
		LAB_constexpr Vector(F const x, F const y, F const z) : x{ x }, y{ y }, z{ z } {}
		LAB_constexpr Vector(F const all) : x{ all }, y{ all }, z{ all } {}

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
			Vector ret = *this;
			ret += other;
			return ret;
		}
		LAB_constexpr void operator-=(Vector const other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
		LAB_constexpr Vector operator-(Vector const other) const {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		LAB_constexpr void operator*=(F const multiplier) {
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;
		}
		LAB_constexpr Vector operator*(F const multiplier) const {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		LAB_constexpr void operator/=(F const divisor) {
			x /= divisor;
			y /= divisor;
			z /= divisor;
		}
		LAB_constexpr Vector operator/(F const divisor) const {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}
		LAB_constexpr Vector& operator*=(Vector const other){
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		LAB_constexpr Vector operator*(Vector const other) const {
			Vector ret = *this;
			ret *= other;
			return ret;
		}
		
		LAB_constexpr F SquaredMagnitude() const {
			return x * x + y * y + z * z;
		}

		LAB_constexpr F Magnitude() const {
			return SupportingMath::Sqrt(SquaredMagnitude());
		}

		LAB_constexpr Vector& Normalize() {
			const auto mag = Magnitude();
			operator/=(mag);
			return *this;
		}
		LAB_constexpr F DotProduct(Vector const& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		LAB_constexpr static Vector Forward() {
			return { F(1), F(0), F(0) };
		}
		LAB_constexpr static Vector Up() {
			return { F(0), F(0), F(1) };
		}
		LAB_constexpr static Vector Right() {
			return { F(0), F(1), F(0) };
		}
	};

	template<std::floating_point F>
	struct Vector<F, 4> {
		F x;
		F y;
		F z;
		F w;

		LAB_constexpr Vector() {}
		LAB_constexpr Vector(F x, F y, F z, F w) : x{ x }, y{ y }, z{ z }, w{ w } {}
		LAB_constexpr Vector(F all) : x{ all }, y{ all }, z{ all }, w{ all } {}

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
			else if (row == 3) {
				return w;
			}
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
			else if (row == 3) {
				return w;
			}
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
			Vector ret = *this;
			ret += other;
			return ret;
		}
		LAB_constexpr void operator-=(Vector const other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		LAB_constexpr Vector operator-(Vector const other) const {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		LAB_constexpr void operator*=(F const multiplier) {
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;
			w *= multiplier;
		}
		LAB_constexpr Vector operator*(F const multiplier) const {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		LAB_constexpr void operator/=(F const divisor) {
			x /= divisor;
			y /= divisor;
			z /= divisor;
			w /= divisor;
		}
		LAB_constexpr Vector operator/(F const divisor) const {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}
		LAB_constexpr Vector& operator*=(Vector const other){
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return *this;
		}
		LAB_constexpr Vector operator*(Vector const other) const {
			Vector ret = *this;
			ret *= other;
			return ret;
		}

		LAB_constexpr F SquaredMagnitude() const {
			return x * x + y * y + z * z + w * w;
		}

		LAB_constexpr F Magnitude() const {
			return SupportingMath::Sqrt(SquaredMagnitude());
		}

		LAB_constexpr void Normalize() {
			const auto mag = Magnitude();
			operator/=(mag);
		}

		LAB_constexpr F DotProduct(Vector const& other) const {
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		LAB_constexpr static Vector Forward() {
			return { F(1), F(0), F(0), F(0) };
		}
		LAB_constexpr static Vector Up() {
			return { F(0), F(0), F(1), F(0) };
		}
		LAB_constexpr static Vector Right() {
			return { F(0), F(1), F(0), F(0) };
		}
		LAB_constexpr static Vector Ahead() {
			return { F(0), F(0), F(0), F(1) };
		}
	};

	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F DotProduct(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		F sum = first.x * second.x + first.y * second.y;
		if constexpr (Dimensions >= 3) {
			sum += first.z * second.z;
		}
		if constexpr (Dimensions >= 4) {
			sum += first.w * second.w;
		}
		return sum;
	}
	//according to my testing, this is 28% faster than a conventional DimensionsDot(Normalize, Normalize)
	//that test was before i knew reverse square root was faster than square root. should be faster now
	template<std::floating_point F, uint8_t Dimensions>
	LAB_constexpr F NormalizedDotProduct(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		const F combinedMagSquared = first.SquaredMagnitude() * second.SquaredMagnitude();
		if (combinedMagSquared != F(0)) {	
			if constexpr (Dimensions == 2) {
				const F numerator = first.x * second.x + first.y * second.y;
				if (numerator > F(0)) {
					//return SupportingMath::Sqrt(numerator * numerator / combinedMagSquared);
					return SupportingMath::InverseSqrt(combinedMagSquared / (numerator * numerator));
				}
				else if (numerator < F(0)) {
					//return -SupportingMath::Sqrt(numerator * numerator / combinedMagSquared);
					return -SupportingMath::InverseSqrt(combinedMagSquared / (numerator * numerator));
				}
			}
			else if constexpr (Dimensions == 3) {
				const F numerator = first.x * second.x + first.y * second.y + first.z * second.z;
				if (numerator > F(0)) {
					//return SupportingMath::Sqrt(numerator * numerator / combinedMagSquared);
					return SupportingMath::InverseSqrt(combinedMagSquared / (numerator * numerator));
				}
				else if (numerator < F(0)) {
					//return -SupportingMath::Sqrt(numerator * numerator / combinedMagSquared);
					return -SupportingMath::InverseSqrt(combinedMagSquared / (numerator * numerator));
				}
			}
			else if constexpr (Dimensions == 4) {
				const F numerator = first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
				if (numerator > F(0)) {
					return SupportingMath::InverseSqrt(combinedMagSquared / (numerator * numerator));
				}
				else if (numerator < F(0)) {
					return -SupportingMath::InverseSqrt(combinedMagSquared / (numerator * numerator));
				}
			}
		}
		return F(0);
	}


	template<std::floating_point F>
	LAB_constexpr F CrossProduct(Vector<F, 2> const first, Vector<F, 2> const second) {
		return first.x * second.y - first.y * second.x;
	}

	template<std::floating_point F>
	LAB_constexpr Vector<F, 3> CrossProduct(Vector<F, 3> const first, Vector<F, 3> const second) {
		//inlining matrix
		return Vector<F, 3>{
			first.y * second.z - first.z * first.y,
			first.z * second.x - first.x * second.z,
			first.x * second.y - first.y * second.x
		};
	}

	//look up https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process for the 4th dimensional Orthogonal
}


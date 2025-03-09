#pragma once

#include "SupportingMath.h"

#include <concepts>

namespace Linear_Algebra {
	//F short for floating point, can be double or float, Dimensions is short for dimensions
	template<std::floating_point F, std::uint8_t Dimensions> requires((Dimensions > 1) && (Dimensions <= 4))
	struct Vector{};

	template<std::floating_point F>
	struct Vector<F, 2> {
		F x;
		F y;
		constexpr Vector() {}
		constexpr Vector(F const x, F const y) : x{ x }, y{ y } {}
		constexpr Vector(F const all) : x{ all }, y{ all } {}

		constexpr F& operator[](uint8_t const row) {
			//static_assert(row < 2);
			return *(&x + row);
		}


		constexpr bool operator==(Vector const other) const {
			return (x == other.x) && (y == other.y);
		}
		constexpr void operator+= (Vector const other) {
			x += other.x;
			y += other.y;
		}
		constexpr Vector operator+(Vector const other) const {
			Vector ret = *this;
			ret += other;
			return ret;
		}
		constexpr void operator-=(Vector const other) {
			x -= other.x;
			y -= other.y;
		}
		constexpr Vector operator-(Vector const other) const {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		constexpr void operator *=(F const multiplier) {
			x *= multiplier;
			y *= multiplier;
		}
		constexpr Vector operator*(F const multiplier) const {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		constexpr void operator /=(F const divisor) {
			x /= divisor;
			y /= divisor;
		}
		constexpr Vector operator /(F const divisor) const {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}
		
		constexpr F SquaredMagnitude() const {
			return x * x + y * y;
		}

		constexpr F Magnitude() const {
			return SupportingMath::Sqrt(SquaredMagnitude());
		}

		constexpr void Normalize() {
			const auto mag = Magnitude();
			operator/=(mag);
		}
	};


	template<std::floating_point F>
	struct Vector<F, 3> {
		F x;
		F y;
		F z;
		constexpr Vector() {}
		constexpr Vector(F const x, F const y, F const z) : x{ x }, y{ y }, z{ z } {}
		constexpr Vector(F const all) : x{ all }, y{ all }, z{ all } {}

		constexpr F& operator[](uint8_t const row) {
			//static_assert(row < 3);
			return *(&x + row);
		}


		constexpr bool operator==(Vector const other) const {
			return (x == other.x) && (y == other.y) && (z == other.z);
		}
		constexpr void operator += (Vector const other) {
			x += other.x;
			y += other.y;
			z += other.z;
		}
		constexpr Vector operator+(Vector const other) const {
			Vector ret = *this;
			ret += other;
			return ret;
		}
		constexpr void operator-=(Vector const other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}
		constexpr Vector operator-(Vector const other) const {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		constexpr void operator *=(F const multiplier) {
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;
		}
		constexpr Vector operator*(F const multiplier) const {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		constexpr void operator /=(F const divisor) {
			x /= divisor;
			y /= divisor;
			z /= divisor;
		}
		constexpr Vector operator /(F const divisor) const {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}
		
		constexpr F SquaredMagnitude() const {
			return x * x + y * y + z * z;
		}

		constexpr F Magnitude() const {
			return SupportingMath::Sqrt(SquaredMagnitude());
		}

		constexpr void Normalize() {
			const auto mag = Magnitude();
			operator/=(mag);
		}
	};

	template<std::floating_point F>
	struct Vector<F, 4> {
		F x;
		F y;
		F z;
		F w;
		constexpr Vector() {}
		constexpr Vector(F x, F y, F z, F w) : x{ x }, y{ y }, z{ z }, w{ w } {}
		constexpr Vector(F all) : x{ all }, y{ all }, z{ all }, w{ all } {}

		constexpr F& operator[](uint8_t const row) {
			//static_assert(row < 4);
			return *(&x + row);
		}
		constexpr bool operator==(Vector const other) const {
			return (x == other.x) && (y == other.y) && (z == other.z) && (w == other.w);
		}
		constexpr void operator+=(Vector const other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		constexpr Vector operator+(Vector const other) const {
			Vector ret = *this;
			ret += other;
			return ret;
		}
		constexpr void operator-=(Vector const other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}
		constexpr Vector operator-(Vector const other) const {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		constexpr void operator*=(F const multiplier) {
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;
			w *= multiplier;
		}
		constexpr Vector operator*(F const multiplier) const {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		constexpr void operator/=(F const divisor) {
			x /= divisor;
			y /= divisor;
			z /= divisor;
			w /= divisor;
		}
		constexpr Vector operator/(F const divisor) const {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}

		constexpr F SquaredMagnitude() const {
			return x * x + y * y + z * z + w * w;
		}

		constexpr F Magnitude() const {
			return SupportingMath::Sqrt(SquaredMagnitude());
		}

		constexpr void Normalize() {
			const auto mag = Magnitude();
			operator/=(mag);
		}
	};

	template<std::floating_point F, uint8_t Dimensions>
	constexpr F DimensionsDotProduct(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
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
	template<std::floating_point F, uint8_t Dimensions>
	constexpr F NormalizedDimensionsDotProduct(Vector<F, Dimensions> const first, Vector<F, Dimensions> const second) {
		const F combinedMagSquared = first.SquaredMagitude() * second.SquaredMagnitude();
		if (combinedMagSquared != F(0)) {	
			F numerator = first.x * second.x + first.y * second.y;
			if constexpr (Dimensions >= 3) {
				numerator += first.z * second.z;
			}
			if constexpr (Dimensions >= 4) {
				numerator += first.w * second.w;
			}
			if (numerator > F(0)) {
				return SupportingMath::Sqrt(numerator * numerator / combinedMagSquared);
			}
			else if (numerator < F(0)) {
				return -SupportingMath::Sqrt(numerator * numerator / combinedMagSquared);
			}
		}
		return F(0);
	}


	template<std::floating_point F>
	constexpr F CrossProduct(Vector<F, 2> const first, Vector<F, 2> const second) {
		return first.x * second.y - first.y * second.x;
	}

	template<std::floating_point F>
	constexpr Vector<F, 3> CrossProduct(Vector<F, 3> const first, Vector<F, 3> const second) {
		//inlining matrix
		return Vector{
			first.y * second.z - first.z * first.y,
			first.z * second.x - first.x * second.z,
			first.x * second.y - first.y * second.x
		};
	}

	//look up https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process for the 4th dimensional Orthogonal
}


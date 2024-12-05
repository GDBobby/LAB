#pragma once

#include "SupportingMath.h"

#include <array>
#include <concepts>

namespace Linear_Algebra {
	//F short for floating point, can be double or float, D is short for dimensions
	template<std::floating_point F, std::uint8_t D> requires((D > 0) && (D <= 4))
	struct Vector;

	template<std::floating_point F>
	struct Vector<F, 1> {
		F x;
		Vector() {}
		Vector(F x) : x{ x } {

		}

		{//artificial scope, for code editors
		//operators in this scope
		// doing operators for a 1 dimensional vectors seems somewhat pointless
			constexpr F& operator[](uint8_t row) {
				static_assert(row < D);
				return *(&x + row);
			}


			constexpr void operator += (Vector const& other) {
				x += other.x;
			}
			constexpr Vector operator+(Vector const& other) {
				Vector ret = *this;
				ret += other;
				return ret;
			}
			constexpr void operator-=(Vector const& other) {
				x -= x;
			}
			constexpr Vector operator-(Vector const& other) {
				Vector ret = *this;
				ret -= other;
				return ret;
			}
			constexpr void operator *=(F multiplier) {
				x *= multiplier;
			}
			constexpr Vector operator*(F multiplier) {
				Vector ret = *this;
				ret *= multiplier;
				return ret;
			}
			constexpr void operator /=(F divisor) {
				x /= divisor;
			}
			constexpr Vector operator /(F divisor) {
				Vector ret = *this;
				ret /= divisor;
				return ret;
			}
		}

		constexpr F SqrMagnitude() {
			return x * x;
		}

		constexpr F Magnitude() {
#if 1//PSEUDO BRANCHING
			return x * ((x < 0.f) * -1.f);
#else
			if constexpr (x < 0.f) {
				return x * -1.f;
			}
			return x;
#endif
		}

		constexpr void Normalize() {
			const auto mag = Magnitude();
			x /= mag;
		}
	};

	template<std::floating_point F>
	struct Vector<F, 2> {
		F x;
		F y;
		Vector() {}
		Vector(F x, F y) : x{ x }, y{ y } {}
		Vector(F all) : x{ all }, y{ all } {}


		{//artificial scope, for code editors
		//operators in this scope
			constexpr F& operator[](uint8_t row) {
				static_assert(row < D);
				return *(&x + row);
			}


			constexpr void operator += (Vector const& other) {
				x += other.x;
				y += other.y;
			}
			constexpr Vector operator+(Vector const& other) {
				Vector ret = *this;
				ret += other;
				return ret;
			}
			constexpr void operator-=(Vector const& other) {
				x -= x;
				y -= y;
			}
			constexpr Vector operator-(Vector const& other) {
				Vector ret = *this;
				ret -= other;
				return ret;
			}
			constexpr void operator *=(F multiplier) {
				x *= multiplier;
				y *= multiplier;
			}
			constexpr Vector operator*(F multiplier) {
				Vector ret = *this;
				ret *= multiplier;
				return ret;
			}
			constexpr void operator /=(F divisor) {
				x /= divisor;
				y /= divisor;
			}
			constexpr Vector operator /(F divisor) {
				Vector ret = *this;
				ret /= divisor;
				return ret;
			}
		}
		constexpr F SqrMagnitude() {
			return x * x + y * y;
		}

		constexpr F Magnitude() {
			return sqrt(SqrtMagnitude());
		}

		constexpr void Normalize() {
			const auto mag = Magnitude();
			x /= mag;
		}
	};


	template<std::floating_point F>
	struct Vector<F, 3> {
		F x;
		F y;
		F z;
		Vector() {}
		Vector(F x, F y, F z) : x{ x }, y{ y }, z{ z } {}
		Vector(F all) : x{ all }, y{ all }, z{ all } {}

		{//artificial scope, for code editors
		//operators in this scope
			constexpr F& operator[](uint8_t row) {
				static_assert(row < D);
				return *(&x + row);
			}


			constexpr void operator += (Vector const& other) {
				x += other.x;
				y += other.y;
				z += other.z;
			}
			constexpr Vector operator+(Vector const& other) {
				Vector ret = *this;
				ret += other;
				return ret;
			}
			constexpr void operator-=(Vector const& other) {
				x -= x;
				y -= y;
				z -= z;
			}
			constexpr Vector operator-(Vector const& other) {
				Vector ret = *this;
				ret -= other;
				return ret;
			}
			constexpr void operator *=(F multiplier) {
				x *= multiplier;
				y *= multiplier;
				z *= multiplier;
			}
			constexpr Vector operator*(F multiplier) {
				Vector ret = *this;
				ret *= multiplier;
				return ret;
			}
			constexpr void operator /=(F divisor) {
				x /= divisor;
				y /= divisor;
				z /= divisor;
			}
			constexpr Vector operator /(F divisor) {
				Vector ret = *this;
				ret /= divisor;
				return ret;
			}
		}
		constexpr F SqrMagnitude() {
			return x * x + y * y + z * z;
		}

		constexpr F Magnitude() {
			return sqrt(SqrtMagnitude());
		}
	};

	template<std::floating_point F>
	struct Vector<F, 4> {
		F x;
		F y;
		F z;
		F w;
		Vector() {}
		Vector(F x, F y, F z, F w) : x{ x }, y{ y }, z{ z }, w{ w } {}
		Vector(F all) : x{ all }, y{ all }, z{ all }, w{ all } {}

{//artificial scope, for code editors
//operators in this scope
		constexpr F& operator[](uint8_t row) {
			static_assert(row < D);
			return *(&x + row);
		}


		constexpr void operator += (Vector const& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		constexpr Vector operator+(Vector const& other) {
			Vector ret = *this;
			ret += other;
			return ret;
		}
		constexpr void operator-=(Vector const& other) {
			x -= x;
			y -= y;
			z -= z;
			w -= w;
		}
		constexpr Vector operator-(Vector const& other) {
			Vector ret = *this;
			ret -= other;
			return ret;
		}
		constexpr void operator *=(F multiplier) {
			x *= multiplier;
			y *= multiplier;
			z *= multiplier;
			w *= multiplier;
		}
		constexpr Vector operator*(F multiplier) {
			Vector ret = *this;
			ret *= multiplier;
			return ret;
		}
		constexpr void operator /=(F divisor) {
			x /= divisor;
			y /= divisor;
			z /= divisor;
			w /= divisor;
		}
		constexpr Vector operator /(F divisor) {
			Vector ret = *this;
			ret /= divisor;
			return ret;
		}
}
		constexpr F SqrMagnitude() {
			return x * x + y * y + z * z + w * w;
		}

		constexpr F Magnitude() {
			return sqrt(SqrtMagnitude());
		}
	};
}


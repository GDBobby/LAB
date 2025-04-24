#pragma once
#include "MatrixTxT.h"


#pragma once

#include "Vector.h"
#include "Debugging.h"
#include <concepts>
#include <type_traits> //can i remove this?
#include <array> //i want to replace this with initializer list
#include <cstring> //for memcpy

namespace lab {
	//im moving all TxT, T == T into their own separate files, so branching for square matrices not requried anymore
	template<std::floating_point F> 
	struct Matrix<F, 2, 2, 2> {
		
		Vector<F, 4> columns;

		LAB_constexpr Matrix() : columns{} {}

		//identity matrix construction
		LAB_constexpr Matrix(F const initVal) :
            columns{initVal, F(0), F(0), initVal}
        {}

		LAB_constexpr Matrix(Matrix const& other) : 
            columns{other.columns}
        {}

		LAB_constexpr Matrix& operator=(Matrix const& other){
            columns = other.columns;
			return *this;
		}

		LAB_constexpr explicit Matrix(Vector<F, 4> const vec) : columns{vec} {}
        LAB_constexpr explicit Matrix(Vector<F, 2> const vec0, Vector<F, 2> const vec1) : columns{vec0, vec1} {}

		LAB_constexpr F& At(const uint8_t column, const uint8_t row) {
#if LAB_DEBUGGING_ACCESS
			assert((column < 2) && (row < 2));
#endif
			return columns[column * 2 + row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
#if LAB_DEBUGGING_ACCESS
			assert((column < 2) && (row < 2));
#endif
			return columns[column * 2 + row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
#if LAB_DEBUGGING_ACCESS
			assert(index < 4);
#endif
			return columns[index];
		}
		LAB_constexpr F operator[](const uint8_t index) const {
#if LAB_DEBUGGING_ACCESS
			assert(index < 4);
#endif
			return columns[index];
		}

		LAB_constexpr Matrix& operator*=(Matrix<F, 2, 2, 2> const& other) const {
			columns *= other.columns;
            return *this;
		}

		LAB_constexpr bool operator==(Matrix<F, 2, 2, 2> const& other) const{
			return columns == other.columns;
		}

		LAB_constexpr Vector<F, 2> operator*(Vector<F, 2> const vector) const {
            //ret[0] += columns[0] * vector[0];
            //ret[0] += columns[2] * vector[1];
            //ret[1] += columns[1] * vector[0];
            //ret[1] += columns[3] * vector[1];
            return{
                columns.x * vector.x + columns.z * vector.y,
                columns.y * vector.x + columns.w * vector.y
            };
		}

		LAB_constexpr Matrix operator*(Matrix<F, 2, 2, 2> const& other) const {
            //Matrix ret;
            //ret.At(0, 0) = At(0, 0) * other.At(0, 0) + At(1, 0) * other.At(0, 1);
            //ret.At(0, 1) = At(0, 1) * other.At(0, 0) + At(1, 1) * other.At(0, 1);

            //ret.At(1, 0) = At(0, 0) * other.At(1, 0) + At(1, 0) * other.At(1, 1);
            //ret.At(1, 1) = At(0, 1) * other.At(1, 0) + At(1, 1) * other.At(1, 1);

            return{
                Vector<F, 4>{
                    columns.x * other.columns.x + columns.z * other.columns.y,
                    columns.y * other.columns.x + columns.w * other.columns.y,
                    columns.x * other.columns.z + columns.z * other.columns.w,
                    columns.y * other.columns.z + columns.w * other.columns.w
                }
            };
		}
		
		LAB_constexpr Matrix& operator*=(F const multiplier) {
            columns *= multiplier;
			return *this;
		}
		LAB_constexpr Matrix operator*(F const multiplier) const {
            return Matrix{
                columns * multiplier
            };
		}
		LAB_constexpr Matrix& operator/=(F const divider) {
#if LAB_DEBUGGING_FLOAT_ANOMALY
            //handle divider == 0
#endif
			columns /= divider;
			return *this;
		}
		LAB_constexpr Matrix operator/(F const divider) const {
#if LAB_DEBUGGING_FLOAT_ANOMALY
            //handle divider == 0
#endif
			return Matrix{
                columns / divider
            };
		}

		LAB_constexpr Matrix<F, 2, 2, 2> Transposed() const {
            return {
                Vector<F, 4>{
                    columns.w,
                    columns.z,
                    columns.y,
                    columns.x
                }
            };
		}


		LAB_constexpr F GetDeterminant() const {
            return columns.x * columns.w - columns.z * columns.y;
		}

		LAB_constexpr Matrix Invert() {
			const F determinent = GetDeterminant();
#if LAB_DEBUGGING_FLOAT_ANOMALY
            assert(determinent != F(0));
#endif
			return operator/=(determinent);
		}

		LAB_constexpr Matrix GetInverse() const {
			const F determinent = GetDeterminant();
#if LAB_DEBUGGING_FLOAT_ANOMALY
            assert(determinent != F(0));
#endif
			return operator/(determinent);
		}
	};
}//namespace Linear_Algebra


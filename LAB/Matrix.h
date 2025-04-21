#pragma once

#include "Vector.h"
#include "Debugging.h"

#include <cassert>
#include <concepts>
#include <utility>
#include <type_traits>
#include <array>

namespace LAB {
	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t ColumnAlignment = Rows> 
	requires((Rows <= 4) && (Rows > 1) && 
			(Columns <= 4) && (Columns > 1) && 
			(ColumnAlignment >= Rows))
	struct Matrix {
		
		Vector<F, ColumnAlignment> columns[Columns];

		LAB_constexpr Matrix() : columns{} {}

		LAB_constexpr Matrix(F const initValue) {
			if constexpr(Columns == 2){
				columns[0] = Vector<F, ColumnAlignment>{initValue};
				columns[1] = Vector<F, ColumnAlignment>{initValue};
			}
			else if constexpr (Columns == 3){
				columns[0] = Vector<F, ColumnAlignment>{initValue};
				columns[1] = Vector<F, ColumnAlignment>{initValue};
				columns[2] = Vector<F, ColumnAlignment>{initValue};

			}
			else if constexpr (Columns == 4){
				columns[0] = Vector<F, ColumnAlignment>{initValue};
				columns[1] = Vector<F, ColumnAlignment>{initValue};
				columns[2] = Vector<F, ColumnAlignment>{initValue};
				columns[3] = Vector<F, ColumnAlignment>{initValue};

			}
		}
		LAB_constexpr Matrix(Matrix const& other){
			for (uint8_t i = 0; i < Columns; i++){
				columns[i] = other.columns[i];
			}
		}
		LAB_constexpr Matrix& operator=(Matrix const& other){
			for (uint8_t i = 0; i < Columns; i++){
				columns[i] = other.columns[i];
			}
			return *this;
		}


		LAB_constexpr explicit Matrix(std::array<Vector<F, Rows>, Columns> const& vectors) {
			//assert(vectors.size() == Columns);

			for (uint8_t column = 0; column < Columns; ++column) {
				columns[column] = vectors[column];
			}
		}

		LAB_constexpr F& At(const uint8_t column, const uint8_t row) {
			assert((column < Columns) && (row < Rows));
			//return data[column * ColumnAlignment + row];
			return columns[column][row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
			assert((column < Columns) && (row < Rows));
			//return data[column * ColumnAlignment + row];
			return columns[column][row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
			LAB_constexpr uint8_t row = index % ColumnAlignment;
			LAB_constexpr uint8_t column = (index - row) / ColumnAlignment;
			assert((column < Columns) && (row < Rows));

			return At(column, row);
		}
		LAB_constexpr F operator[](const uint8_t index) const {
			LAB_constexpr uint8_t row = index % ColumnAlignment;
			LAB_constexpr uint8_t column = (index - row) / ColumnAlignment;
			assert((column < Columns) && (row < Rows));

			return At(column, row);
		}

		template<uint8_t Alignment>
		LAB_constexpr Matrix& operator*=(Matrix<F, Columns, Rows, Alignment> const& other) const {
			Matrix tempCopy = *this;
			*this = tempCopy * other;
			return *this;
		}

		template<uint8_t Alignment>
		LAB_constexpr Matrix operator*(Matrix<F, Columns, Rows, Alignment> const& other) const {
			if constexpr(Columns == 3 && Rows == 3) {
				
				Matrix ret;
				ret.columns[0][0] = columns[0][0] * other.columns[0][0] + columns[1][0] * other.columns[0][1] + columns[2][0] * other.columns[0][2];
				ret.columns[0][1] = columns[0][1] * other.columns[0][0] + columns[1][1] * other.columns[0][1] + columns[2][1] * other.columns[0][2];
				ret.columns[0][2] = columns[0][2] * other.columns[0][0] + columns[1][2] * other.columns[0][1] + columns[2][2] * other.columns[0][2];

				ret.columns[1][0] = columns[0][0] * other.columns[1][0] + columns[1][0] + other.columns[1][1] + columns[2][0] * other.columns[1][2];
				ret.columns[1][1] = columns[0][1] * other.columns[1][0] + columns[1][1] + other.columns[1][1] + columns[2][1] * other.columns[1][2];
				ret.columns[1][2] = columns[0][2] * other.columns[1][0] + columns[1][2] + other.columns[1][1] + columns[2][2] * other.columns[1][2];

				ret.columns[2][0] = columns[0][0] * other.columns[2][0] + columns[1][0] + other.columns[2][1] + columns[2][0] * other.columns[2][2];
				ret.columns[2][1] = columns[0][1] * other.columns[2][0] + columns[1][1] + other.columns[2][1] + columns[2][1] * other.columns[2][2];
				ret.columns[2][2] = columns[0][2] * other.columns[2][0] + columns[1][2] + other.columns[2][1] + columns[2][2] * other.columns[2][2];
				return ret;
			}
			else if constexpr(Columns == 4 && Rows == 4){
				if (std::is_constant_evaluated()) {
					Matrix ret{F(0)};
					ret.columns[0][0] = columns[0][0] * other.columns[0][0] + columns[1][0] * other.columns[0][1] + columns[2][0] * other.columns[0][2] + columns[3][0] * other.columns[0][3];
					ret.columns[0][1] = columns[0][1] * other.columns[0][0] + columns[1][1] * other.columns[0][1] + columns[2][1] * other.columns[0][2] + columns[3][1] * other.columns[0][3];
					ret.columns[0][2] = columns[0][2] * other.columns[0][0] + columns[1][2] * other.columns[0][1] + columns[2][2] * other.columns[0][2] + columns[3][2] * other.columns[0][3];
					ret.columns[0][3] = columns[0][3] * other.columns[0][0] + columns[1][3] * other.columns[0][1] + columns[2][3] * other.columns[0][2] + columns[3][3] * other.columns[0][3];
					
					ret.columns[1][0] = columns[0][0] * other.columns[1][0] + columns[1][0] + other.columns[1][1] + columns[2][0] * other.columns[1][2] + columns[3][0] * other.columns[1][3];
					ret.columns[1][1] = columns[0][1] * other.columns[1][0] + columns[1][1] + other.columns[1][1] + columns[2][1] * other.columns[1][2] + columns[3][1] * other.columns[1][3];
					ret.columns[1][2] = columns[0][2] * other.columns[1][0] + columns[1][2] + other.columns[1][1] + columns[2][2] * other.columns[1][2] + columns[3][2] * other.columns[1][3];
					ret.columns[1][3] = columns[0][3] * other.columns[1][0] + columns[1][3] * other.columns[1][1] + columns[2][3] * other.columns[1][2] + columns[3][3] * other.columns[1][3];
					
					ret.columns[2][0] = columns[0][0] * other.columns[2][0] + columns[1][0] + other.columns[2][1] + columns[2][0] * other.columns[2][2] + columns[3][0] * other.columns[2][3];
					ret.columns[2][1] = columns[0][1] * other.columns[2][0] + columns[1][1] + other.columns[2][1] + columns[2][1] * other.columns[2][2] + columns[3][1] * other.columns[2][3];
					ret.columns[2][2] = columns[0][2] * other.columns[2][0] + columns[1][2] + other.columns[2][1] + columns[2][2] * other.columns[2][2] + columns[3][2] * other.columns[2][3];
					ret.columns[0][3] = columns[0][3] * other.columns[2][0] + columns[1][3] * other.columns[2][1] + columns[2][3] * other.columns[2][2] + columns[3][3] * other.columns[2][3];
					
					ret.columns[3][0] = columns[0][0] * other.columns[3][0] + columns[1][0] + other.columns[3][1] + columns[2][0] * other.columns[3][2] + columns[3][0] * other.columns[3][3];
					ret.columns[3][1] = columns[0][1] * other.columns[3][0] + columns[1][1] + other.columns[3][1] + columns[2][1] * other.columns[3][2] + columns[3][1] * other.columns[3][3];
					ret.columns[3][2] = columns[0][2] * other.columns[3][0] + columns[1][2] + other.columns[3][1] + columns[2][2] * other.columns[3][2] + columns[3][2] * other.columns[3][3];
					ret.columns[3][3] = columns[0][3] * other.columns[3][0] + columns[1][3] * other.columns[3][1] + columns[2][3] * other.columns[3][2] + columns[3][3] * other.columns[3][3];
					return ret;
				}
				else{
					Matrix ret;
					for(uint8_t i = 0; i < 4; ++i){
						const __m128 otherCol0 = _mm_set1_ps(other.columns[i].x);
						const __m128 otherCol1 = _mm_set1_ps(other.columns[i].y);
						const __m128 otherCol2 = _mm_set1_ps(other.columns[i].z);
						const __m128 otherCol3 = _mm_set1_ps(other.columns[i].w);
					
						const __m128 mul0 = _mm_mul_ps(columns[0].vec, otherCol0);
						const __m128 mul1 = _mm_mul_ps(columns[1].vec, otherCol1);
						const __m128 mul2 = _mm_mul_ps(columns[2].vec, otherCol2);
						const __m128 mul3 = _mm_mul_ps(columns[3].vec, otherCol3);
					
						__m128 col = mul0;
						col = _mm_add_ps(col, mul1);
						col = _mm_add_ps(col, mul2);
						col = _mm_add_ps(col, mul3);

						ret.columns[i].vec = col;
					}
					return ret;
				}
			}
		}


		template<uint8_t Alignment>
		LAB_constexpr bool operator==(Matrix<F, Columns, Rows, Alignment> const& other) const{
			for(uint8_t y = 0; y < Rows; y++){
				for(uint8_t x = 0; x < Columns; x++){
					if(At(x, y) != other.At(x, y)){
						return false;
					}
				}
			}
			return true;	
		}

		LAB_constexpr Vector<F, Rows> operator*(Vector<F, Columns> const vector) const {
			Vector<F, Rows> ret{};
			for (uint8_t row = 0; row < Rows; row++) {
				for (uint8_t column = 0; column < Columns; column++) {
					ret[row] += columns[column][row] * vector[column];
				}
			}
			return ret;
		}
	};

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	requires(Rows > 2)
	[[nodiscard]] LAB_constexpr auto Translate(Matrix<F, Columns, Rows, Alignment> const& matrix, Vector<F, Rows - 1> const translation) {
		Matrix<F, Columns, Rows, Alignment> ret = matrix;
		for (uint8_t row = 0; row < (Rows - 1); row++) {
			for (uint8_t column = 0; column < (Columns - 1); column++) {
				ret.columns[3][row] += matrix.columns[column][row] * translation[column];
			}
		}
		return ret;
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	requires(Rows > 2)
	[[nodiscard]] LAB_constexpr auto Rotate(Matrix<F, Columns, Rows, Alignment> const& matrix, F const angle, Vector<F, Rows - 1> const axis) {
		F const cosine = SupportingMath::Cos(angle);
		F const sine = SupportingMath::Sin(angle);
#if MATH_DEBUGGING
		//ensure vec is normalized
#endif
		const Vector<F, 3> temp{ axis * (F(1) - cosine) };
		Matrix<F, 3, 3> rotation; //its a 4x4 matrix, but the outer parts dont matter
		rotation.columns[0][0] = cosine + temp.x * axis.x;
		rotation.columns[0][1] = temp.x * axis.y + sine * axis.z;
		rotation.columns[0][2] = temp.x * axis.z - sine * axis.y;

		rotation.columns[1][0] = temp.y * axis.x - sine * axis.z;
		rotation.columns[1][1] = cosine + temp.y * axis.y;
		rotation.columns[1][2] = temp.y * axis.z + sine * axis.x;

		rotation.columns[2][0] = temp.z * axis.x + sine * axis.y;
		rotation.columns[2][1] = temp.z * axis.y - sine * axis.x;
		rotation.columns[2][2] = cosine + temp.z * axis.z;

		Matrix<F, Columns, Rows, Alignment> retMat = matrix;
		for (uint8_t column = 0; column < (Columns - 1); column++) {
			for (uint8_t row = 0; row < Rows; row++) {
				//need an intermediate copy
				retMat.At(column, row) = matrix.At(0, row) * rotation.At(column, 0) + matrix.At(1, row) * rotation.At(column, 1) + matrix.At(2, row) * rotation.At(column, 2);
			}
		}

		return retMat;
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	requires(Rows > 2)
	[[nodiscard]] LAB_constexpr auto Scale(Matrix<F, Columns, Rows, Alignment> const& matrix, Vector<F, Rows - 1> const scalingVec) {
		Matrix<F, Columns, Rows, Alignment> retMat = matrix;
		for (uint8_t column = 0; column < (Columns - 1); column++) {
			for (uint8_t row = 0; row < Rows; row++) {
				retMat.At(column, row) *= scalingVec[column];
			}
		}
		return retMat;
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Alignment = Columns>
	[[nodiscard]] LAB_constexpr Matrix<F, Columns, Columns, Alignment> Identity(F const initVal) {
		//static_assert(Rows == Columns && "can not use Identity unless rows and Columns are equal");
		Matrix<F, Columns, Columns, Alignment> ret{ 0.f };

		for (uint8_t i = 0; i < Columns; i++) {
			ret.At(i, i) = initVal;
		}

		return ret;
	}
}//namespace Linear_Algebra


#pragma once

#include "Vector.h"
#include "Debugging.h"

#include <cassert>
#include <concepts>
#include <utility>
#include <type_traits>
#include <array>
#include <cstring>

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
			if(std::is_constant_evaluated()){
				for (uint8_t i = 0; i < Columns; i++){
					columns[i] = other.columns[i];
				}
			}
			else{
				std::memcpy(columns, other.columns, sizeof(F) * Columns * ColumnAlignment);
			}
		}
		LAB_constexpr Matrix& operator=(Matrix const& other){
			if(std::is_constant_evaluated()){
				for (uint8_t i = 0; i < Columns; i++){
					columns[i] = other.columns[i];
				}
			}
			else{
				memcpy(columns, other.columns, sizeof(F) * Columns * ColumnAlignment);
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
		LAB_constexpr bool operator==(Matrix<F, Columns, Rows, Alignment> const& other) const{
			for(uint8_t y = 0; y < Rows; y++){
				for(uint8_t x = 0; x < Columns; x++){
					if(columns[x][y] != other[x][y]){
						return false;
					}
				}
			}
			return true;	
		}


		LAB_constexpr Vector<F, Rows> operator*(Vector<F, Columns> const vector) const {
			if constexpr (std::is_constant_evaluated()){
				if constexpr(Rows == 4 && Columns == 4){
					const Vector<F, 4> mul0 = columns[0] * vector.x;
					const Vector<F, 4> mul1 = columns[1] * vector.y;
					const Vector<F, 4> mul2 = columns[2] * vector.z;
					const Vector<F, 4> mul3 = columns[3] * vector.w;
					return mul0 + mul1 + mul2 + mul3;
				}
				else {
					Vector<F, Rows> ret{F(0)};
					for (uint8_t row = 0; row < Rows; row++) {
						for (uint8_t column = 0; column < Columns; column++) {
							ret[row] += columns[column][row] * vector[column];
						}
					}
					return ret;
				}
			}
			else if constexpr (Rows == 4 && Columns == 4) {
				//copying glm implementation

				const __m128 Mul0 = __mm_mul_ps(columns[0].vec, _mm_set1_ps(vector.x));
				const __m128 Mul1 = __mm_mul_ps(columns[1].vec, _mm_set1_ps(vector.y));
				const __m128 Mul2 = __mm_mul_ps(columns[2].vec, _mm_set1_ps(vector.z));
				const __m128 Mul3 = __mm_mul_ps(columns[3].vec, _mm_set1_ps(vector.w));
				return Vector<F, Rows>{_mm_add_ps(_mm_add_ps(Mul0, Mul1), _mm_add_ps(Mul2, Mul3))};
			}
			//this is an error for any size other than 4/4 i think
			Unreachable();
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
				//if i can get the initializer list constructor working i can condense this a little bit
				Matrix ret;
				//this is the matrix * vector operator
				ret.columns[0] = this->operator*(other.columns[0]);
				ret.columns[1] = this->operator*(other.columns[1]);
				ret.columns[2] = this->operator*(other.columns[2]);
				ret.columns[3] = this->operator*(other.columns[3]);
				return ret;
			}
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


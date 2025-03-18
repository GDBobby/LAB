#pragma once

#include "Vector.h"
#include "Debugging.h"

#include <cassert>
#include <concepts>
#include <utility>
#include <type_traits>

namespace LAB {
	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t ColumnAlignment = Rows> 
		requires((Rows <= 4) && (Rows > 1) && 
			(Columns <= 4) && (Columns > 1) && 
			(ColumnAlignment >= Rows))
		struct Matrix {

		F data[ColumnAlignment * Columns];

		LAB_constexpr Matrix() : data{} {}

		LAB_constexpr Matrix(F const initValue) {
			for (uint8_t y = 0; y < Rows; y++) {
				for (uint8_t x = 0; x < Columns; x++) {
					At(x, y) = initValue;
				}
			}
			
		}
		LAB_constexpr F& At(const uint8_t column, const uint8_t row) {
			assert((column < Columns) && (row < Rows));
			return data[column * ColumnAlignment + row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
			assert((column < Columns) && (row < Rows));
			return data[column * ColumnAlignment + row];
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
			Matrix ret;
			if constexpr(Columns == 3 && Rows == 3) {
				ret.At(0, 0) = At(0, 0) * other.At(0, 0) + At(1, 0) * other.At(0, 1) + At(2, 0) * other.At(0, 2);
				ret.At(0, 1) = At(0, 1) * other.At(0, 0) + At(1, 1) * other.At(0, 1) + At(2, 1) * other.At(0, 2);
				ret.At(0, 2) = At(0, 2) * other.At(0, 0) + At(1, 2) * other.At(0, 1) + At(2, 2) * other.At(0, 2);

				ret.At(1, 0) = At(0, 0) * other.At(1, 0) + At(1, 0) + other.At(1, 1) + At(2, 0) * other.At(1, 2);
				ret.At(1, 1) = At(0, 1) * other.At(1, 0) + At(1, 1) + other.At(1, 1) + At(2, 1) * other.At(1, 2);
				ret.At(1, 2) = At(0, 2) * other.At(1, 0) + At(1, 2) + other.At(1, 1) + At(2, 2) * other.At(1, 2);

				ret.At(2, 0) = At(0, 0) * other.At(2, 0) + At(1, 0) + other.At(2, 1) + At(2, 0) * other.At(2, 2);
				ret.At(2, 1) = At(0, 1) * other.At(2, 0) + At(1, 1) + other.At(2, 1) + At(2, 1) * other.At(2, 2);
				ret.At(2, 2) = At(0, 2) * other.At(2, 0) + At(1, 2) + other.At(2, 1) + At(2, 2) * other.At(2, 2);
			}
			else if constexpr(Columns == 4 && Rows == 4){
				ret.At(0, 0) = At(0, 0) * other.At(0, 0) + At(1, 0) * other.At(0, 1) + At(2, 0) * other.At(0, 2) + At(3, 0) * other.At(0, 3);
				ret.At(0, 1) = At(0, 1) * other.At(0, 0) + At(1, 1) * other.At(0, 1) + At(2, 1) * other.At(0, 2) + At(3, 1) * other.At(0, 3);
				ret.At(0, 2) = At(0, 2) * other.At(0, 0) + At(1, 2) * other.At(0, 1) + At(2, 2) * other.At(0, 2) + At(3, 2) * other.At(0, 3);
				ret.At(0, 2) = At(0, 3) * other.At(0, 0) + At(1, 3) * other.At(0, 1) + At(2, 3) * other.At(0, 2) + At(3, 3) * other.At(0, 3);
				
				ret.At(1, 0) = At(0, 0) * other.At(1, 0) + At(1, 0) + other.At(1, 1) + At(2, 0) * other.At(1, 2) + At(3, 0) * other.At(1, 3);
				ret.At(1, 1) = At(0, 1) * other.At(1, 0) + At(1, 1) + other.At(1, 1) + At(2, 1) * other.At(1, 2) + At(3, 1) * other.At(1, 3);
				ret.At(1, 2) = At(0, 2) * other.At(1, 0) + At(1, 2) + other.At(1, 1) + At(2, 2) * other.At(1, 2) + At(3, 2) * other.At(1, 3);
				ret.At(1, 3) = At(0, 3) * other.At(1, 0) + At(1, 3) * other.At(1, 1) + At(2, 3) * other.At(1, 2) + At(3, 3) * other.At(1, 3);
				
				ret.At(2, 0) = At(0, 0) * other.At(2, 0) + At(1, 0) + other.At(2, 1) + At(2, 0) * other.At(2, 2) + At(3, 0) * other.At(2, 3);
				ret.At(2, 1) = At(0, 1) * other.At(2, 0) + At(1, 1) + other.At(2, 1) + At(2, 1) * other.At(2, 2) + At(3, 1) * other.At(2, 3);
				ret.At(2, 2) = At(0, 2) * other.At(2, 0) + At(1, 2) + other.At(2, 1) + At(2, 2) * other.At(2, 2) + At(3, 2) * other.At(2, 3);
				ret.At(0, 2) = At(0, 3) * other.At(2, 0) + At(1, 3) * other.At(2, 1) + At(2, 3) * other.At(2, 2) + At(3, 3) * other.At(2, 3);
				
				ret.At(2, 0) = At(0, 0) * other.At(3, 0) + At(1, 0) + other.At(3, 1) + At(2, 0) * other.At(3, 2) + At(3, 0) * other.At(3, 3);
				ret.At(2, 1) = At(0, 1) * other.At(3, 0) + At(1, 1) + other.At(3, 1) + At(2, 1) * other.At(3, 2) + At(3, 1) * other.At(3, 3);
				ret.At(2, 2) = At(0, 2) * other.At(3, 0) + At(1, 2) + other.At(3, 1) + At(2, 2) * other.At(3, 2) + At(3, 2) * other.At(3, 3);
				ret.At(0, 2) = At(0, 3) * other.At(3, 0) + At(1, 3) * other.At(3, 1) + At(2, 3) * other.At(3, 2) + At(3, 3) * other.At(3, 3);
				
			}

			return ret;
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
	};

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	LAB_constexpr auto FillColumn(Matrix<F, Columns, Rows, Alignment>& mat, const uint8_t col, Vector<F, Rows> const& vector) {
		mat.data[col * Rows] = vector.x;
		mat.data[col * Rows + 1] = vector.y;
		if constexpr (Rows >= 3) {
			mat.data[col * Rows + 2] = vector.z;
		}
		if constexpr (Rows == 4) {
			mat.data[col * Rows + 3] = vector.w;
		}
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment, typename... Vectors>
	LAB_constexpr auto ApplyVectorToMatrix(Matrix<F, Columns, Rows, Alignment>& mat, Vectors&&... vectors) {
		uint8_t col = 1;
		(FillColumn(mat, col++, std::forward<Vectors>(vectors)), ...);
	}

	template<std::floating_point F, uint8_t Rows, uint8_t Alignment, typename... Vectors>
	[[nodiscard]] LAB_constexpr auto CreateMatrix(const Vector<F, Rows> vec1, Vectors&&... vectors) {
		Matrix<F, 1 + sizeof...(vectors), Rows, Alignment> ret;

		FillColumn(ret, 0, vec1);

		if constexpr (sizeof...(vectors) > 0) {
			ApplyVectorToMatrix(ret, std::forward<Vectors>(vectors)...);
		}

		return ret;
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	requires(Rows > 2)
	[[nodiscard]] LAB_constexpr Matrix<F, Columns, Rows, Alignment> Translate(Matrix<F, Columns, Rows, Alignment> const& matrix, Vector<F, Rows - 1> const translation) {
		Matrix<F, Columns, Rows, Alignment> ret;
		for (uint8_t row = 0; row < (Rows - 1); row++) {
			for (uint8_t column = 0; column < (Columns - 1); column++) {
				ret.At(Columns - 1, row) += matrix.At(column, row) * translation[column];
			}
		}
		return ret;
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	requires(Rows > 2)
	[[nodiscard]] LAB_constexpr Matrix<F, Columns, Rows, Alignment> Rotate(Matrix<F, Columns, Rows, Alignment> const& matrix, F const angle, Vector<F, Rows - 1> const axis) {
		F const cosine = SupportingMath::Cos(angle);
		F const sine = SupportingMath::Sin(angle);
#if MATH_DEBUGGING
		//ensure vec is normalized
#endif
		const Vector<F, 3> temp{ axis * (F(1) - cosine) };
		Matrix<F, 3, 3> rotation; //its a 4x4 matrix, but the outer parts dont matter
		rotation.At(0, 0) = cosine + temp.x * axis.x;
		rotation.At(0, 1) = temp.x * axis.y + sine * axis.z;
		rotation.At(0, 2) = temp.x * axis.z - sine * axis.y;

		rotation.At(1, 0) = temp.y * axis.x - sine * axis.z;
		rotation.At(1, 1) = cosine + temp.y * axis.y;
		rotation.At(1, 2) = temp.y * axis.z + sine * axis.x;

		rotation.At(2, 0) = temp.z * axis.x + sine * axis.y;
		rotation.At(2, 1) = temp.z * axis.y - sine * axis.x;
		rotation.At(2, 2) = cosine + temp.z * axis.z;

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
	[[nodiscard]] LAB_constexpr Matrix<F, Columns, Rows, Alignment> Scale(Matrix<F, Columns, Rows, Alignment> const& matrix, Vector<F, Rows - 1> const scalingVec) {
		for (uint8_t column = 0; column < (Columns - 1); column++) {
			for (uint8_t row = 0; row < Rows; row++) {
				matrix.At(column, row) *= scalingVec[column];
			}
		}
		return matrix;
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


#pragma once

#include "Vector.h"
#include <cassert>
#include <concepts>

namespace Linear_Algebra {
	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t ColumnAlignment = (sizeof(float) * Rows)> requires((Rows <= 4) && (Rows > 1) && (Columns <= 4) && (Columns > 1) && (ColumnAlignment >= (Rows * sizeof(float))))
		struct Matrix {

		F data[ColumnAlignment / sizeof(float) * Columns];

		constexpr Matrix() : data{} {}

		template<bool Identity = false> requires((!Identity) || (Identity && (Rows == Columns)))
		Matrix(F const initValue) {
			if constexpr (Identity) {
				for (uint8_t y = 0; y < Rows; y++) {
					for (uint8_t x = 0; x < Columns; x++) {
						At(y, x) = (F)0;
					}
				}
				for (uint8_t i = 0; i < Columns; i++) {
					At(i, i) = initValue;
				}
			}
			else {
				for (uint8_t y = 0; y < Rows; y++) {
					for (uint8_t x = 0; x < Columns; x++) {
						At(y, x) = initValue;
					}
				}
			}
		}

		constexpr F& At(const uint8_t column, const uint8_t row) {
			assert((column < Columns) && (row < Rows));
			return data[column * Rows + row];
		}
		constexpr F& operator[](const uint8_t index) {
			constexpr uint8_t row = index % Rows;
			constexpr uint8_t column = (index - row) / Rows;
			assert((column < Columns) && (row < Rows));

			return At(column, row);
		}
		constexpr F At(const uint8_t column, const uint8_t row) const {
			assert((column < Columns) && (row < Rows));
			return data[column * Rows + row];
		}
		constexpr F operator[](const uint8_t index) const {
			constexpr uint8_t row = index % Rows;
			constexpr uint8_t column = (index - row) / Rows;
			assert((column < Columns) && (row < Rows));

			return At(column, row);
		}

		template<uint8_t Alignment>
		constexpr Matrix operator*(Matrix<F, Columns, Rows, Alignment> const& other) const {
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
	};

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t Alignment>
	constexpr auto FillColumn(Matrix<F, Columns, Rows, Alignment>& mat, const uint8_t col, Vector<F, Rows> const& vector) {
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
	constexpr auto ApplyVectorToMatrix(Matrix<F, Columns, Rows, Alignment>& mat, Vectors&&... vectors) {
		uint8_t col = 1;
		(FillColumn(mat, col++, std::forward<Vectors>(vectors)), ...);
	}

	template<std::floating_point F, uint8_t Rows, uint8_t Alignment, typename... Vectors>
	constexpr auto CreateMatrix(const Vector<F, Rows> vec1, Vectors&&... vectors) {
		Matrix<F, 1 + sizeof...(vectors), Rows, Alignment> ret;

		FillColumn(ret, 0, vec1);

		if constexpr (sizeof...(vectors) > 0) {
			ApplyVectorToMatrix(ret, std::forward<Vectors>(vectors)...);
		}

		return ret;
	}
}//namespace Linear_Algebra


#pragma once

#include "Vector.h"
#include <cassert>

namespace Linear_Algebra {
	template<std::floating_point F, uint8_t Columns, uint8_t Rows> requires((Rows <= 4) && (Rows > 1) && (Columns <= 4) && (Columns > 1))
		struct Matrix {
		std::array<F, Rows * Columns> data;

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
	};

	template<std::floating_point F, uint8_t Columns, uint8_t Rows>
	constexpr auto FillColumn(Matrix<F, Columns, Rows>& mat, const uint8_t col, Vector<F, Rows> const& vector) {
		mat.data[col * Rows] = vector.x;
		mat.data[col * Rows + 1] = vector.y;
		if constexpr (Rows >= 3) {
			mat.data[col * Rows + 2] = vector.z;
		}
		if constexpr (Rows == 4) {
			mat.data[col * Rows + 3] = vector.w;
		}
	}

	template<std::floating_point F, uint8_t Columns, uint8_t Rows, typename... Vectors>
	constexpr auto ApplyVectorToMatrix(Matrix<F, Columns, Rows>& mat, Vectors&&... vectors) {
		uint8_t col = 1;
		(FillColumn(mat, col++, std::forward<Vectors>(vectors)), ...);
	}

	template<std::floating_point F, uint8_t Rows, typename... Vectors>
	constexpr auto CreateMatrix(const Vector<F, Rows> vec1, Vectors&&... vectors) {
		Matrix<F, 1 + sizeof...(vectors), Rows> ret;

		FillColumn(ret, 0, vec1);

		if constexpr (sizeof...(vectors) > 0) {
			ApplyVectorToMatrix(ret, std::forward<Vectors>(vectors)...);
		}

		return ret;
	}
}//namespace Linear_Algebra


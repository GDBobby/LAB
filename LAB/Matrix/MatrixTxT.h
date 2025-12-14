#pragma once

#include "../Vector.h"
#include "../Debugging.h"
#include <concepts>
#include <type_traits> //can i remove this?
#include <array> //i want to replace this with initializer list
#include <cstring> //for memcpy

namespace lab {
	//im moving all TxT, T == T into their own separate files, so branching for square matrices not requried anymore
	template<std::floating_point F, uint8_t Columns, uint8_t Rows, uint8_t ColumnAlignment = Rows> 
	requires((Rows <= 4) && (Rows > 1) && 
			(Columns <= 4) && (Columns > 1) && 
			(ColumnAlignment >= Rows) && (ColumnAlignment <= 4))
	struct Matrix {
		
		Vector<F, ColumnAlignment> columns[Columns];

		LAB_constexpr Matrix() : columns{} {}

		//identity matrix construction
		LAB_constexpr Matrix(F const initVal) {
			for(uint8_t column = 0; column < Columns; column++){
				columns[column] = Vector<F, Rows>(initVal);
			}
		}
		LAB_constexpr Matrix(Matrix const& other){
			if constexpr (std::is_constant_evaluated()){
				for (uint8_t i = 0; i < Columns; i++){
					columns[i] = other.columns[i];
				}
			}
			else{
				std::memcpy(columns, other.columns, sizeof(columns[0]) * ColumnAlignment);
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
#if LAB_DEBUGGING_ACCESS
			assert((column < Columns) && (row < Rows));
#endif
			return columns[column][row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
#if LAB_DEBUG_LEVEL >= LAB_DEBUG_ASSERT_ACCESS
			assert((column < Columns) && (row < Rows));
#endif
			return columns[column][row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
			const uint8_t row = index % ColumnAlignment;
			const uint8_t column = (index - row) / ColumnAlignment;
#if LAB_DEBUGGING_ACCESS
			assert((column < Columns) && (row < Rows));
#endif

			return columns[column][row];
		}
		LAB_constexpr F operator[](const uint8_t index) const {
			const uint8_t row = index % ColumnAlignment;
			const uint8_t column = (index - row) / ColumnAlignment;
#if LAB_DEBUGGING_ACCESS
			assert((column < Columns) && (row < Rows));
#endif

			return columns[column][row];
		}

		template<uint8_t Alignment>
		LAB_constexpr bool operator==(Matrix<F, Columns, Rows, Alignment> const& other) const{
			for(uint8_t y = 0; y < Rows; y++){
				for(uint8_t x = 0; x < Columns; x++){
					if(columns[x][y] != other.columns[x][y]){
						return false;
					}
				}
			}
			return true;	
		}

		LAB_constexpr Vector<F, Rows> operator*(Vector<F, Columns> const vector) const {
			Vector<F, Rows> ret{F(0)};
			for (uint8_t row = 0; row < Rows; row++) {
				for (uint8_t column = 0; column < Columns; column++) {
					ret[row] += columns[column][row] * vector[column];
				}
			}
			return ret;
		}

		template<uint8_t Alignment>
		LAB_constexpr Matrix operator*(Matrix<F, Columns, Rows, Alignment> const& other) const {
			
		}

		template<uint8_t Alignment>
		LAB_constexpr Matrix& operator*=(Matrix<F, Columns, Rows, Alignment> const& other) const {
			return *this = *this * other;
		}
		
		LAB_constexpr Matrix& operator*=(F const multiplier) {
			for(uint8_t column = 0; column < Columns; column++){
				columns[column] *= multiplier;
			}
			return *this;
		}
		LAB_constexpr Matrix operator*(F const multiplier) const {
			Matrix ret = *this;
			return ret *= multiplier;
		}
		LAB_constexpr Matrix& operator/=(F const divider) {
			for(uint8_t column = 0; column < Columns; column++){
				columns[column] /= divider;
			}
			return *this;
		}
		LAB_constexpr Matrix operator/(F const divider) const {
			Matrix ret = *this;
			return ret /= divider;
		}

		template<uint8_t Alignment = Columns>
		LAB_constexpr Matrix<F, Rows, Columns, Alignment> Transposed() const {
			Matrix<F, Rows, Columns, Alignment> ret{};
			for(uint8_t column = 0; column < Columns; column++){
				for(uint8_t row = 0; row < Rows; row++){
					ret.columns[row][column] = columns[column][row];
				}
			}
			return ret;
		}
	};

}//namespace Linear_Algebra


#pragma once
#include "MatrixTxT.h"

#include "Vector.h"
#include "Debugging.h"
#ifdef LAB_DEBUGGING_FLOAT_ANOMALIES
#include <cassert>
#endif
#include <concepts>
#include <type_traits> //can i remove this?
#include <cstring> //for memcpy

namespace lab {
	//im moving all TxT, T == T into their own separate files, so branching for square matrices not requried anymore
	template<std::floating_point F>
	struct Matrix<F, 3, 3, 3> {
		
		Vector<F, 3> columns[3];

		LAB_constexpr Matrix() : columns{} {}

		//identity matrix construction
		LAB_constexpr Matrix(F const initVal) :
            columns{
                Vector<F, 3>(initVal, F(0), F(0)),
                Vector<F, 3>(F(0), initVal, F(0)),
                Vector<F, 3>(F(0), F(0), initVal)
            }
        {}

		LAB_constexpr Matrix(Matrix const& other) :
            columns{
                other.columns[0],
                other.columns[1],
                other.columns[2]
            }
        {}

		LAB_constexpr Matrix& operator=(Matrix const& other){
			if(std::is_constant_evaluated()){
				for (uint8_t i = 0; i < 3; i++){
					columns[i] = other.columns[i];
				}
			}
			else{
				memcpy(columns, other.columns, sizeof(F) * 3 * 3);
			}
			return *this;
		}

		LAB_constexpr explicit Matrix(Vector<F, 3> const vec0, Vector<F, 3> const vec1, Vector<F, 3> const vec2) : 
        columns{
            vec0,
            vec1,
            vec2
        }
        {}

		LAB_constexpr F& At(const uint8_t column, const uint8_t row) {
#if LAB_DEBUG_LEVEL >= 1
			assert((column < 3) && (row < 3));
#endif
			return columns[column][row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
#if LAB_DEBUG_LEVEL >= 1
			assert((column < 3) && (row < 3));
#endif
			return columns[column][row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
			const uint8_t row = index % 3;
			const uint8_t column = (index - row) / 3;
#if LAB_DEBUG_LEVEL >= 1
			assert((column < 3) && (row < 3));
#endif
			return columns[column][row];
		}
		LAB_constexpr F operator[](const uint8_t index) const {
			const uint8_t row = index % 3;
			const uint8_t column = (index - row) / 3;
#if LAB_DEBUG_LEVEL >= 1
            assert((column < 3) && (row < 3));
#endif

			return columns[column][row];
		}



		template<uint8_t Alignment>
		LAB_constexpr bool operator==(Matrix<F, 3, 3, Alignment> const& other) const{
			for(uint8_t y = 0; y < 3; y++){
				for(uint8_t x = 0; x < 3; x++){
					if(columns[x][y] != other.columns[x][y]){
						return false;
					}
				}
			}
			return true;	
		}

		LAB_constexpr Vector<F, 3> operator*(Vector<F, 3> const vector) const {
                Vector<F, 3> ret{F(0)};
                for (uint8_t row = 0; row < 3; row++) {
                    for (uint8_t column = 0; column < 3; column++) {
                        ret[row] += columns[column][row] * vector[column];
                    }
                }
                return ret;
		}

		template<uint8_t Alignment>
		LAB_constexpr Matrix operator*(Matrix<F, 3, 3, Alignment> const& other) const {
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
		
		template<uint8_t Alignment>
		LAB_constexpr Matrix& operator*=(Matrix<F, 3, 3, Alignment> const& other) const {
			return *this = *this * other;
		}

		LAB_constexpr Matrix& operator*=(F const multiplier) {
			for(uint8_t column = 0; column < 3; column++){
                columns[column] *= multiplier;
			}
			return *this;
		}
		LAB_constexpr Matrix operator*(F const multiplier) const {
            return Matrix{
                columns[0] * multiplier,
                columns[1] * multiplier,
                columns[2] * multiplier
            };
		}
		LAB_constexpr Matrix& operator/=(F const divider) {
			for(uint8_t column = 0; column < 3; column++){
                columns[column] /= divider;
			}
			return *this;
		}
		LAB_constexpr Matrix operator/(F const divider) const {
            return Matrix{
                columns[0] / divider,
                columns[1] / divider,
                columns[2] / divider
            };
		}

		LAB_constexpr Matrix<F, 3, 3, 3> Transposed() const {
			Matrix<F, 3, 3, 3> ret;
			for(uint8_t column = 0; column < 3; column++){
				for(uint8_t row = 0; row < 3; row++){
					ret.columns[row][column] = columns[column][row];
				}
			}
			return ret;
		}


		LAB_constexpr F GetDeterminant() const {
            const F first = columns[0][0] * (columns[1][1] * columns[2][2] - columns[2][1] * columns[2][1]);
            const F second =columns[1][0] * (columns[0][1] * columns[2][2] - columns[2][1] * columns[2][0]);
            const F third = columns[2][0] * (columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2]);

            return first - second + third;
		}

		LAB_constexpr Matrix Invert() {
			const F determinent = GetDeterminant();
#if LAB_DEBUG_LEVEL >= 1
            assert(determinent != F(0));
#endif
			return operator/=(determinent);
		}

		LAB_constexpr Matrix GetInverse() const {
			const F determinent = GetDeterminant();
#if LAB_DEBUG_LEVEL >= 1
            assert(determinent != F(0));
#endif
			return operator/(determinent);
		}

        Matrix GetTranslated(Vector<F, 2> const transVec) const {
            Matrix<F, 3, 3, 3> ret = *this;
            for (uint8_t row = 0; row < 2; row++) {
                ret.columns[3][row] += columns[0][row] * transVec.x;
                ret.columns[3][row] += columns[1][row] * transVec.y;
            }
            return ret;
        }

        Matrix GetRotated(F const angle, Vector<F, 2> const axis) const {
            F const cosine = Cos(angle);
            F const sine = Sin(angle);
            const Vector<F, 2> temp{ axis * (F(1) - cosine) };

            //not implemented yet
            return Matrix{F(0)};
        }

        
        Matrix GetScaled(Vector<F, 2> const scalingVec) const{
            Matrix<F, 3, 3, 3> retMat = *this;
            for(uint8_t row = 0; row < 3; row++){
                retMat.columns[0][row] *= scalingVec.x;
                retMat.columns[1][row] *= scalingVec.y;
            }
            
        }
	};

    template<std::floating_point F>
    struct Matrix<F, 3, 3, 4> {
		
		Vector<F, 4> columns[3];

		LAB_constexpr Matrix() : columns{} {}

		//identity matrix construction
		LAB_constexpr Matrix(F const initVal) :
            columns{
                Vector<F, 4>(initVal, F(0), F(0), F(0)),
                Vector<F, 4>(F(0), initVal, F(0), F(0)),
                Vector<F, 4>(F(0), F(0), initVal, F(0))
            }
        {}

		LAB_constexpr Matrix(Matrix const& other) :
            columns{
                other.columns[0],
                other.columns[1],
                other.columns[2]
            }
        {}

		LAB_constexpr Matrix& operator=(Matrix const& other){
			if(std::is_constant_evaluated()){
				for (uint8_t i = 0; i < 3; i++){
					columns[i] = other.columns[i];
				}
			}
			else{
				memcpy(columns, other.columns, sizeof(F) * 3 * 4);
			}
			return *this;
		}

		LAB_constexpr explicit Matrix(Vector<F, 3> const vec0, Vector<F, 3> const vec1, Vector<F, 3> const vec2) : 
        columns{
            Vector<F, 4>(vec0, F(0)),
            Vector<F, 4>(vec1, F(0)),
            Vector<F, 4>(vec2, F(0))
        }
        {}

		LAB_constexpr F& At(const uint8_t column, const uint8_t row) {
#if LAB_DEBUG_LEVEL >= 1
			assert((column < 3) && (row < 3));
#endif
			return columns[column][row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
#if LAB_DEBUG_LEVEL >= 1
			assert((column < 3) && (row < 3));
#endif
			return columns[column][row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
			const uint8_t row = index % 3;
			const uint8_t column = (index - row) / 3;
#if LAB_DEBUG_LEVEL >= 1
			assert((column < 3) && (row < 3));
#endif
			return columns[column][row];
		}
		LAB_constexpr F operator[](const uint8_t index) const {
			const uint8_t row = index % 3;
			const uint8_t column = (index - row) / 3;
#if LAB_DEBUG_LEVEL >= 1
            assert((column < 3) && (row < 3));
#endif

			return columns[column][row];
		}



		template<uint8_t Alignment>
		LAB_constexpr bool operator==(Matrix<F, 3, 3, Alignment> const& other) const{
			for(uint8_t y = 0; y < 3; y++){
				for(uint8_t x = 0; x < 3; x++){
					if(columns[x][y] != other.columns[x][y]){
						return false;
					}
				}
			}
			return true;	
		}

		LAB_constexpr Vector<F, 3> operator*(Vector<F, 3> const vector) const {
                Vector<F, 3> ret{F(0)};
                for (uint8_t row = 0; row < 3; row++) {
                    for (uint8_t column = 0; column < 3; column++) {
                        ret[row] += columns[column][row] * vector[column];
                    }
                }
                return ret;
		}

		template<uint8_t Alignment>
		LAB_constexpr Matrix operator*(Matrix<F, 3, 3, Alignment> const& other) const {
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
		
		template<uint8_t Alignment>
		LAB_constexpr Matrix& operator*=(Matrix<F, 3, 3, Alignment> const& other) const {
			return *this = *this * other;
		}

		LAB_constexpr Matrix& operator*=(F const multiplier) {
			for(uint8_t column = 0; column < 3; column++){
                columns[column] *= multiplier;
			}
			return *this;
		}
		LAB_constexpr Matrix operator*(F const multiplier) const {
            return Matrix{
                columns[0] * multiplier,
                columns[1] * multiplier,
                columns[2] * multiplier
            };
		}
		LAB_constexpr Matrix& operator/=(F const divider) {
#if LAB_DEBUG_LEVEL >= 2
            //handle divider == 0
#endif
			for(uint8_t column = 0; column < 3; column++){
                columns[column] /= divider;
			}
			return *this;
		}
		LAB_constexpr Matrix operator/(F const divider) const {
#if LAB_DEBUG_LEVEL >= 2
			//handle divider == 0
#endif
            return Matrix{
                columns[0] / divider,
                columns[1] / divider,
                columns[2] / divider
            };
		}

		LAB_constexpr Matrix Transposed() const {
			Matrix ret;
			for(uint8_t column = 0; column < 3; column++){
				for(uint8_t row = 0; row < 3; row++){
					ret.columns[row][column] = columns[column][row];
				}
			}
			if constexpr (std::is_constant_evaluated()){
				//cant leave these undefined in const eval
				ret.columns[0][3] = F(0);
				ret.columns[1][3] = F(0);
				ret.columns[2][3] = F(0);
				ret.columns[3][3] = F(0);
			}
			return ret;
		}


		LAB_constexpr F GetDeterminant() const {
            const F first = columns[0][0] * (columns[1][1] * columns[2][2] - columns[2][1] * columns[2][1]);
            const F second =columns[1][0] * (columns[0][1] * columns[2][2] - columns[2][1] * columns[2][0]);
            const F third = columns[2][0] * (columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2]);

            return first - second + third;
		}

		LAB_constexpr Matrix Invert() {
			const F determinent = GetDeterminant();
#if LAB_DEBUG_LEVEL >= 1
            assert(determinent != F(0));
#endif
			return operator/=(determinent);
		}

		LAB_constexpr Matrix GetInverse() const {
			const F determinent = GetDeterminant();
#if LAB_DEBUG_LEVEL >= 1
            assert(determinent != F(0));
#endif
			return operator/(determinent);
		}

        Matrix GetTranslated(Vector<F, 2> const transVec) const {
            Matrix<F, 3, 3, 3> ret = *this;
            for (uint8_t row = 0; row < 2; row++) {
                ret.columns[3][row] += columns[0][row] * transVec.x;
                ret.columns[3][row] += columns[1][row] * transVec.y;
            }
            return ret;
        }

        Matrix GetRotated(F const angle, Vector<F, 2> const axis) const {
            F const cosine = Cos(angle);
            F const sine = Sin(angle);
            const Vector<F, 2> temp{ axis * (F(1) - cosine) };

            //not implemented yet
            return Matrix{F(0)};
        }

        
        Matrix GetScaled(Vector<F, 2> const scalingVec) const{
            Matrix<F, 3, 3, 3> retMat = *this;
            for(uint8_t row = 0; row < 3; row++){
                retMat.columns[0][row] *= scalingVec.x;
                retMat.columns[1][row] *= scalingVec.y;
            }
            
        }
	};


    
    template<std::floating_point F, uint8_t Alignment>
    requires(Alignment == 3 || Alignment == 4)
    LAB_constexpr Matrix<F, 3, 3, Alignment> Translate(Matrix<F, 3, 3, Alignment> const& matrix, Vector<F, 3> const transVec){
        return matrix.GetTranslated(transVec);
    }
    template<std::floating_point F, uint8_t Alignment>
    requires(Alignment == 3 || Alignment == 4)
    LAB_constexpr Matrix<F, 3, 3, Alignment> Rotate(Matrix<F, 3, 3, Alignment> const& matrix, F const angle, Vector<F, 3> const axis){
        return matrix.GetRotated(angle, axis);
    }
    template<std::floating_point F, uint8_t Alignment>
    requires(Alignment == 3 || Alignment == 4)
    LAB_constexpr Matrix<F, 3, 3, Alignment> Scale(Matrix<F, 3, 3, Alignment> const& matrix, Vector<F, 3> const scaleVec){
        return matrix.GetScaled(scaleVec);
    }
}//namespace Linear_Algebra


#pragma once
#include "MatrixTxT.h"

#include "Vector.h"
#include "Debugging.h"

#include <cassert>
#include <concepts>
#include <utility>
#include <type_traits> //can i remove this?
#include <cstring> //for memcpy

namespace lab {
	//im moving all TxT, T == T into their own separate files, so branching for square matrices not requried anymore

	template<std::floating_point F>
	struct Matrix<F, 4, 4, 4> {

		Vector<F, 4> columns[4];

		LAB_constexpr Matrix() : columns{} {}

		//identity matrix construction
		LAB_constexpr Matrix(F const initVal) : 
            columns{
                Vector<F, 4>(initVal, F(0), F(0), F(0)), 
                Vector<F, 4>(F(0), initVal, F(0), F(0)), 
                Vector<F, 4>(F(0), F(0), initVal, F(0)),
                Vector<F, 4>(F(0), F(0), F(0), initVal)
            } 
            {}

		LAB_constexpr Matrix(Matrix const& other) : 
            columns{
                other.columns[0],
                other.columns[1],
                other.columns[2],
                other.columns[3]
            } 
        {}

		LAB_constexpr Matrix& operator=(Matrix const& other){
			if(std::is_constant_evaluated()){
				for (uint8_t i = 0; i < 4; i++){
					columns[i] = other.columns[i];
				}
			}
			else{
				memcpy(columns, other.columns, sizeof(F) * 4 * 4);
			}
			return *this;
		}

		LAB_constexpr explicit Matrix(Vector<F, 4> const vector0, Vector<F, 4> const vector1, Vector<F, 4> const vector2, Vector<F, 4> const vector3) :
        columns{
            vector0,
            vector1,
            vector2,
            vector3
        } 
        {}

		LAB_constexpr F& At(const uint8_t column, const uint8_t row) {
			assert((column < 4) && (row < 4));
			return columns[column][row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
			assert((column < 4) && (row < 4));
			return columns[column][row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
			const uint8_t row = index % 4;
			const uint8_t column = (index - row) / 4;
			assert((column < 4) && (row < 4));

			return columns[column][row];
		}
		LAB_constexpr F operator[](const uint8_t index) const {
			const uint8_t row = index % 4;
			const uint8_t column = (index - row) / 4;
			assert((column < 4) && (row < 4));

			return columns[column][row];
		}

		LAB_constexpr bool operator==(Matrix<F, 4, 4, 4> const& other) const{
			for(uint8_t column = 0; column < 4; column++){
                if(columns[column] != other.columns[column]){
                    return false;
				}
			}
			return true;	
		}

		LAB_constexpr Vector<F, 4> operator*(Vector<F, 4> const vector) const {
			if constexpr (std::is_constant_evaluated()){
                const Vector<F, 4> mul0 = columns[0] * vector.x;
                const Vector<F, 4> mul1 = columns[1] * vector.y;
                const Vector<F, 4> mul2 = columns[2] * vector.z;
                const Vector<F, 4> mul3 = columns[3] * vector.w;
                return mul0 + mul1 + mul2 + mul3;
			}
			else {
				//copying glm implementation, minor tweaks

				const __m128 Mul0 = __mm_mul_ps(columns[0].vec, _mm_set1_ps(vector.x));
				const __m128 Mul1 = __mm_mul_ps(columns[1].vec, _mm_set1_ps(vector.y));
				const __m128 Mul2 = __mm_mul_ps(columns[2].vec, _mm_set1_ps(vector.z));
				const __m128 Mul3 = __mm_mul_ps(columns[3].vec, _mm_set1_ps(vector.w));
				return Vector<F, 4>{_mm_add_ps(_mm_add_ps(Mul0, Mul1), _mm_add_ps(Mul2, Mul3))};
			}
		}

		LAB_constexpr Matrix operator*(Matrix<F, 4, 4, 4> const& other) const {
            if constexpr (std::is_constant_evaluated()) {
                Matrix ret{};
                //matrix * vector operator
                ret.columns[0] = this->operator*(other.columns[0]);
                ret.columns[1] = this->operator*(other.columns[1]);
                ret.columns[2] = this->operator*(other.columns[2]);
                ret.columns[3] = this->operator*(other.columns[3]);
                return ret;
            }
            else {
                Matrix ret;
                for(uint8_t i = 0; i < 4; ++i){
                    //the matrix * vector operator isnt good here, im assuming because it converts from __m128 to vector to __m128 or something. idk. couldve been a benchmark error
                    const __m128 mul0 = _mm_mul_ps(columns[0].vec, _mm_set1_ps(other.columns[i].x));
                    const __m128 mul1 = _mm_mul_ps(columns[1].vec, _mm_set1_ps(other.columns[i].y));
                    const __m128 mul2 = _mm_mul_ps(columns[2].vec, _mm_set1_ps(other.columns[i].z));
                    const __m128 mul3 = _mm_mul_ps(columns[3].vec, _mm_set1_ps(other.columns[i].w));
                
                    ret.columns[i].vec = _mm_add_ps(_mm_add_ps(mul0, mul1), _mm_add_ps(mul2, mul3));
                }
                return ret;
            }
		}
        
		LAB_constexpr Matrix& operator*=(Matrix<F, 4, 4, 4> const& other) const {
            //its necessary to make a copy unfortunately
            //there might be some shenanigans where i can avoid it, but i suspect not
			return *this = *this * other;
		}
		
		LAB_constexpr Matrix& operator*=(F const multiplier) {
			for(uint8_t column = 0; column < 4; column++){
                columns[column] *= multiplier;
			}
			return *this;
		}
		LAB_constexpr Matrix operator*(F const multiplier) const {
            return Matrix{
                columns[0] * multiplier,
                columns[1] * multiplier,
                columns[2] * multiplier,
                columns[3] * multiplier
            };
		}
		LAB_constexpr Matrix& operator/=(F const divider) {
#if LAB_DEBUG_LEVEL >= 2
            //handle divider == 0
#endif
			for(uint8_t column = 0; column < 4; column++){
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
                columns[2] / divider,
                columns[3] / divider
            };
		}

		LAB_constexpr Matrix<F, 4, 4, 4> Transposed() const {
			Matrix<F, 4, 4, 4> ret;
			for(uint8_t column = 0; column < 4; column++){
				for(uint8_t row = 0; row < 4; row++){
					ret.columns[row][column] = columns[column][row];
				}
			}
			return ret;
		}

    /* for the normal matrix, a 3x3 matrix is extracted, and transpose(inverse(3x3))
		LAB_constexpr F GetDeterminant() const requires(4 == 4)  {

				//ill come back to this later, is there even a use for the 4d determinent in gaming?
			
		}

		LAB_constexpr Matrix Invert() {
			const F determinent = GetDeterminant();
			//if(determinent == F(0)){
				//bad, LAB_debug here
			//}
			return operator/=(determinent);
		}

		LAB_constexpr Matrix GetInverse() const  {
			const F determinent = GetDeterminant();
			//if(determinent == F(0)){
				//bad, LAB_debug here
			//}
			return operator/(determinent);
		}
    */

        LAB_constexpr Matrix GetTranslated(Vector<F, 3> const transVec) const {
            Matrix ret = *this;
            for(uint8_t row = 0; row < 3; row++){
                ret.columns[3][row] += columns[0][row] * transVec.x;
                ret.columns[3][row] += columns[1][row] * transVec.y;
                ret.columns[3][row] += columns[2][row] * transVec.z;
            }
            return ret;
        }
        LAB_constexpr Matrix GetRotated(F const angle, Vector<F, 3> const axis) const{
            F const cosine = Cos(angle);
            F const sine = Sin(angle);
    #if MATH_DEBUGGING
            //ensure axis is normalized
    #endif
            const Vector<F, 3> temp{ axis * (F(1) - cosine) };
            Matrix<F, 3, 3> rotation; //its a 4x4 matrix, but the outer parts dont matter
            rotation.columns[0].x = cosine + temp.x * axis.x;
            rotation.columns[0].y = temp.x * axis.y + sine * axis.z;
            rotation.columns[0].z = temp.x * axis.z - sine * axis.y;
    
            rotation.columns[1].x = temp.y * axis.x - sine * axis.z;
            rotation.columns[1].y = cosine + temp.y * axis.y;
            rotation.columns[1].z = temp.y * axis.z + sine * axis.x;
    
            rotation.columns[2].x = temp.z * axis.x + sine * axis.y;
            rotation.columns[2].y = temp.z * axis.y - sine * axis.x;
            rotation.columns[2].z = cosine + temp.z * axis.z;
    
            Matrix retMat = *this;
            for (uint8_t column = 0; column < 3; column++) {
                for (uint8_t row = 0; row < 4; row++) {
                    //need an intermediate copy
                    retMat.columns[column][row] = columns[0][row] * rotation.columns[column].x 
                                                + columns[1][row] * rotation.columns[column].y 
                                                + columns[2][row] * rotation.columns[column].z;
                }
            }
    
            return retMat;
        }
	
        LAB_constexpr Matrix GetScaled(Vector<F, 3> const scalingVec) const {
            Matrix<F, 4, 4, 4> retMat = *this;
            for (uint8_t column = 0; column < 3; column++) {
                for (uint8_t row = 0; row < 4; row++) {
                    retMat.columns[column][row] *= scalingVec[column];
                }
            }
            return retMat; 
        }
    };

    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4, 4> Translate(Matrix<F, 4, 4, 4> const& matrix, Vector<F, 3> const transVec){
        return matrix.GetTranslated(transVec);
    }
    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4, 4> Rotate(Matrix<F, 4, 4, 4> const& matrix, F const angle, Vector<F, 3> const axis){
        return matrix.GetRotated(angle, axis);
    }
    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4, 4> Scale(Matrix<F, 4, 4, 4> const& matrix, Vector<F, 3> const scaleVec){
        return matrix.GetScaled(scaleVec);
    }


}//namespace Linear_Algebra


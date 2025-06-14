#pragma once
#include "MatrixTxT.h"

#include "Vector.h"
#include "Debugging.h"

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
#if LAB_DEBUGGING_ACCESS
			assert((column < 4) && (row < 4));
#endif
			return columns[column][row];
		}
		LAB_constexpr F At(const uint8_t column, const uint8_t row) const {
#if LAB_DEBUGGING_ACCESS
			assert((column < 4) && (row < 4));
#endif
			return columns[column][row];
		}

		LAB_constexpr F& operator[](const uint8_t index) {
			const uint8_t row = index % 4;
			const uint8_t column = (index - row) / 4;
#if LAB_DEBUGGING_ACCESS
			assert((column < 4) && (row < 4));
#endif

			return columns[column][row];
		}
		LAB_constexpr F operator[](const uint8_t index) const {
			const uint8_t row = index % 4;
			const uint8_t column = (index - row) / 4;
#if LAB_DEBUGGING_ACCESS
			assert((column < 4) && (row < 4));
#endif

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
#ifdef USING_SIMD
			if constexpr (std::is_constant_evaluated()){
#endif
                const Vector<F, 4> mul0 = columns[0] * vector.x;
                const Vector<F, 4> mul1 = columns[1] * vector.y;
                const Vector<F, 4> mul2 = columns[2] * vector.z;
                const Vector<F, 4> mul3 = columns[3] * vector.w;
                return mul0 + mul1 + mul2 + mul3; 
#ifdef USING_SIMD
			}
			else {
				//copying glm implementation, minor tweaks

				const __m128 Mul0 = __mm_mul_ps(columns[0].vec, _mm_set1_ps(vector.x));
				const __m128 Mul1 = __mm_mul_ps(columns[1].vec, _mm_set1_ps(vector.y));
				const __m128 Mul2 = __mm_mul_ps(columns[2].vec, _mm_set1_ps(vector.z));
				const __m128 Mul3 = __mm_mul_ps(columns[3].vec, _mm_set1_ps(vector.w));
				return Vector<F, 4>{_mm_add_ps(_mm_add_ps(Mul0, Mul1), _mm_add_ps(Mul2, Mul3))};
			}
#endif
		}

		LAB_constexpr Matrix operator*(Matrix<F, 4, 4, 4> const& other) const {
#ifdef USING_SIMD
            if constexpr (std::is_constant_evaluated()) {
#endif
                Matrix ret{};
                //matrix * vector operator
                ret.columns[0] = this->operator*(other.columns[0]);
                ret.columns[1] = this->operator*(other.columns[1]);
                ret.columns[2] = this->operator*(other.columns[2]);
                ret.columns[3] = this->operator*(other.columns[3]);
                return ret;
#ifdef USING_SIMD
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
#endif
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
#if LAB_DEBUGGING_FLOAT_ANOMALIES
            //handle divider == 0
#endif
			for(uint8_t column = 0; column < 4; column++){
                columns[column] /= divider;
			}
			return *this;
		}
		LAB_constexpr Matrix operator/(F const divider) const {
#if LAB_DEBUGGING_FLOAT_ANOMALIES
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

        LAB_constexpr F GetDeterminant() const {
            F const SubFactor00 = columns[2][2] * columns[3][3] - columns[3][2] * columns[2][3];
            F const SubFactor01 = columns[2][1] * columns[3][3] - columns[3][1] * columns[2][3];
            F const SubFactor02 = columns[2][1] * columns[3][2] - columns[3][1] * columns[2][2];
            F const SubFactor03 = columns[2][0] * columns[3][3] - columns[3][0] * columns[2][3];
            F const SubFactor04 = columns[2][0] * columns[3][2] - columns[3][0] * columns[2][2];
            F const SubFactor05 = columns[2][0] * columns[3][1] - columns[3][0] * columns[2][1];

            Vector<F, 4> DetCof(
                +(columns[1][1] * SubFactor00 - columns[1][2] * SubFactor01 + columns[1][3] * SubFactor02),
                -(columns[1][0] * SubFactor00 - columns[1][2] * SubFactor03 + columns[1][3] * SubFactor04),
                +(columns[1][0] * SubFactor01 - columns[1][1] * SubFactor03 + columns[1][3] * SubFactor05),
                -(columns[1][0] * SubFactor02 - columns[1][1] * SubFactor04 + columns[1][2] * SubFactor05)
            );

            return
                columns[0][0] * DetCof[0] + columns[0][1] * DetCof[1] +
                columns[0][2] * DetCof[2] + columns[0][3] * DetCof[3];
        }

        LAB_constexpr Matrix& Invert() {
            const F determinent = GetDeterminant();
            //if(determinent == F(0)){
                //bad, LAB_debug here
            //}
            return operator/=(determinent);
        }
        LAB_constexpr Matrix GetInverse() const {
            Matrix inv;

            float invOut[16];

            invOut[0] = At(1, 1) * At(2, 2) * At(3, 3) -
                At(1, 1) * At(2, 3) * At(3, 2) -
                At(2, 1) * At(1, 2) * At(3, 3) +
                At(2, 1) * At(1, 3) * At(3, 2) +
                At(3, 1) * At(1, 2) * At(2, 3) -
                At(3, 1) * At(1, 3) * At(2, 2);

            invOut[1] = -At(0, 1) * At(2, 2) * At(3, 3) +
                At(0, 1) * At(2, 3) * At(3, 2) +
                At(2, 1) * At(0, 2) * At(3, 3) -
                At(2, 1) * At(0, 3) * At(3, 2) -
                At(3, 1) * At(0, 2) * At(2, 3) +
                At(3, 1) * At(0, 3) * At(2, 2);

            invOut[2] = At(0, 1) * At(1, 2) * At(3, 3) -
                At(0, 1) * At(1, 3) * At(3, 2) -
                At(1, 1) * At(0, 2) * At(3, 3) +
                At(1, 1) * At(0, 3) * At(3, 2) +
                At(3, 1) * At(0, 2) * At(1, 3) -
                At(3, 1) * At(0, 3) * At(1, 2);

            invOut[3] = -At(0, 1) * At(1, 2) * At(2, 3) +
                At(0, 1) * At(1, 3) * At(2, 2) +
                At(1, 1) * At(0, 2) * At(2, 3) -
                At(1, 1) * At(0, 3) * At(2, 2) -
                At(2, 1) * At(0, 2) * At(1, 3) +
                At(2, 1) * At(0, 3) * At(1, 2);

            invOut[4] = -At(1, 0) * At(2, 2) * At(3, 3) +
                At(1, 0) * At(2, 3) * At(3, 2) +
                At(2, 0) * At(1, 2) * At(3, 3) -
                At(2, 0) * At(1, 3) * At(3, 2) -
                At(3, 0) * At(1, 2) * At(2, 3) +
                At(3, 0) * At(1, 3) * At(2, 2);

            invOut[5] = At(0, 0) * At(2, 2) * At(3, 3) -
                At(0, 0) * At(2, 3) * At(3, 2) -
                At(2, 0) * At(0, 2) * At(3, 3) +
                At(2, 0) * At(0, 3) * At(3, 2) +
                At(3, 0) * At(0, 2) * At(2, 3) -
                At(3, 0) * At(0, 3) * At(2, 2);

            invOut[6] = -At(0, 0) * At(1, 2) * At(3, 3) +
                At(0, 0) * At(1, 3) * At(3, 2) +
                At(1, 0) * At(0, 2) * At(3, 3) -
                At(1, 0) * At(0, 3) * At(3, 2) -
                At(3, 0) * At(0, 2) * At(1, 3) +
                At(3, 0) * At(0, 3) * At(1, 2);

            invOut[7] = At(0, 0) * At(1, 2) * At(2, 3) -
                At(0, 0) * At(1, 3) * At(2, 2) -
                At(1, 0) * At(0, 2) * At(2, 3) +
                At(1, 0) * At(0, 3) * At(2, 2) +
                At(2, 0) * At(0, 2) * At(1, 3) -
                At(2, 0) * At(0, 3) * At(1, 2);

            invOut[8] = At(1, 0) * At(2, 1) * At(3, 3) -
                At(1, 0) * At(2, 3) * At(3, 1) -
                At(2, 0) * At(1, 1) * At(3, 3) +
                At(2, 0) * At(1, 3) * At(3, 1) +
                At(3, 0) * At(1, 1) * At(2, 3) -
                At(3, 0) * At(1, 3) * At(2, 1);

            invOut[9] = -At(0, 0) * At(2, 1) * At(3, 3) +
                At(0, 0) * At(2, 3) * At(3, 1) +
                At(2, 0) * At(0, 1) * At(3, 3) -
                At(2, 0) * At(0, 3) * At(3, 1) -
                At(3, 0) * At(0, 1) * At(2, 3) +
                At(3, 0) * At(0, 3) * At(2, 1);

            invOut[10] = At(0, 0) * At(1, 1) * At(3, 3) -
                At(0, 0) * At(1, 3) * At(3, 1) -
                At(1, 0) * At(0, 1) * At(3, 3) +
                At(1, 0) * At(0, 3) * At(3, 1) +
                At(3, 0) * At(0, 1) * At(1, 3) -
                At(3, 0) * At(0, 3) * At(1, 1);

            invOut[11] = -At(0, 0) * At(1, 1) * At(2, 3) +
                At(0, 0) * At(1, 3) * At(2, 1) +
                At(1, 0) * At(0, 1) * At(2, 3) -
                At(1, 0) * At(0, 3) * At(2, 1) -
                At(2, 0) * At(0, 1) * At(1, 3) +
                At(2, 0) * At(0, 3) * At(1, 1);

            invOut[12] = -At(1, 0) * At(2, 1) * At(3, 2) +
                At(1, 0) * At(2, 2) * At(3, 1) +
                At(2, 0) * At(1, 1) * At(3, 2) -
                At(2, 0) * At(1, 2) * At(3, 1) -
                At(3, 0) * At(1, 1) * At(2, 2) +
                At(3, 0) * At(1, 2) * At(2, 1);

            invOut[13] = At(0, 0) * At(2, 1) * At(3, 2) -
                At(0, 0) * At(2, 2) * At(3, 1) -
                At(2, 0) * At(0, 1) * At(3, 2) +
                At(2, 0) * At(0, 2) * At(3, 1) +
                At(3, 0) * At(0, 1) * At(2, 2) -
                At(3, 0) * At(0, 2) * At(2, 1);

            invOut[14] = -At(0, 0) * At(1, 1) * At(3, 2) +
                At(0, 0) * At(1, 2) * At(3, 1) +
                At(1, 0) * At(0, 1) * At(3, 2) -
                At(1, 0) * At(0, 2) * At(3, 1) -
                At(3, 0) * At(0, 1) * At(1, 2) +
                At(3, 0) * At(0, 2) * At(1, 1);

            invOut[15] = At(0, 0) * At(1, 1) * At(2, 2) -
                At(0, 0) * At(1, 2) * At(2, 1) -
                At(1, 0) * At(0, 1) * At(2, 2) +
                At(1, 0) * At(0, 2) * At(2, 1) +
                At(2, 0) * At(0, 1) * At(1, 2) -
                At(2, 0) * At(0, 2) * At(1, 1);

            float det = At(0, 0) * invOut[0] + At(0, 1) * invOut[4] + At(0, 2) * invOut[8] + At(0, 3) * invOut[12];

            if (lab::Abs(det) < 1e-6f)
                return {}; // Singular, return identity or zero

            det = 1.0f / det;

            float* dest = reinterpret_cast<float*>(&inv);
            for (int i = 0; i < 16; i++)
                dest[i] = invOut[i] * det;

            return inv;
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


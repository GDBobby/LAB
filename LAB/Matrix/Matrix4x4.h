#pragma once
#include "MatrixTxT.h"

#include "../Vector.h"
#include "../Debugging.h"

#include <concepts>

#ifdef LAB_ROW_MAJOR
#define XM_PERMUTE_PS( v, c ) _mm_shuffle_ps((v), (v), c )
#define XM_FMADD_PS( a, b, c ) _mm_add_ps(_mm_mul_ps((a), (b)), (c))
#endif

namespace lab {
    template<::std::floating_point F>
    struct Matrix<F, 4, 4, 4> {
#ifdef USING_SIMD
        VectorSIMD columns[4];
        using ColType = VectorSIMD;
#else
        Vector<F, 4> columns[4];
        using ColType = Vector<F, 4>;
#endif
        LAB_constexpr Matrix() : columns{} {}

        //identity matrix construction
        [[nodiscard]] explicit LAB_constexpr Matrix(F const initVal) :
            columns{
                ColType(initVal, F(0), F(0), F(0)),
                ColType(F(0), initVal, F(0), F(0)),
                ColType(F(0), F(0), initVal, F(0)),
                ColType(F(0), F(0), F(0), initVal)
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

        LAB_constexpr Matrix& operator=(Matrix const& other) {
            if (std::is_constant_evaluated()) {
                for (uint8_t i = 0; i < 4; i++) {
                    columns[i] = other.columns[i];
                }
            }
            else {
                for (uint8_t i = 0; i < 4; i++) {
                    columns[i] = other.columns[i];
                }
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

        LAB_constexpr bool operator==(Matrix<F, 4, 4, 4> const& other) const {
            for (uint8_t column = 0; column < 4; column++) {
                if (!(columns[column] == other.columns[column])) {
                    return false;
                }
            }
            return true;
        }
#ifndef LAB_ROW_MAJOR
        LAB_constexpr Vector<F, 4> operator*(Vector<F, 4> const vector) const {
#ifdef USING_SIMD
            if constexpr (std::is_constant_evaluated()) {
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
                const VectorSIMD Mul0 = (columns[0].vec * vector.x);
                const VectorSIMD Mul1 = (columns[1].vec * vector.y);
                const VectorSIMD Mul2 = (columns[2].vec * vector.z);
                const VectorSIMD Mul3 = (columns[3].vec * vector.w);
                Vector<F, 4> ret;
                _mm_storeu_ps(&ret, (Mul0 + Mul1) + (Mul2 + Mul3));
                return ret;
            }
#endif
        }
#endif

        LAB_constexpr Matrix operator*(Matrix<F, 4, 4, 4> const& other) const {
#ifdef LAB_ROW_MAJOR
#ifdef USING_SIMD
            if constexpr (!std::is_constant_evaluated()) {
                Matrix mResult;
                Vector<F, 4> vW = columns[0];
                Vector<F, 4> vX = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(0, 0, 0, 0));
                Vector<F, 4> vY = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(1, 1, 1, 1));
                Vector<F, 4> vZ = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(2, 2, 2, 2));
                vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
                // Perform the operation on the first row
                vX = _mm_mul_ps(vX.vec, other.columns[0].vec);
                vY = _mm_mul_ps(vY.vec, other.columns[1].vec);
                vZ = _mm_mul_ps(vZ.vec, other.columns[2].vec);
                vW = _mm_mul_ps(vW.vec, other.columns[3].vec);
                // Perform a binary add to reduce cumulative errors
                vX = _mm_add_ps(vX.vec, vZ.vec);
                vY = _mm_add_ps(vY.vec, vW.vec);
                vX = _mm_add_ps(vX.vec, vY.vec);

                // Repeat for the other 3 rows  
                vW = columns[1];
                vX = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(0, 0, 0, 0));
                vY = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(1, 1, 1, 1));
                vZ = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(2, 2, 2, 2));
                vW = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(3, 3, 3, 3));

                vX = _mm_mul_ps(vX.vec, other.columns[0].vec);
                vY = _mm_mul_ps(vY.vec, other.columns[1].vec);
                vZ = _mm_mul_ps(vZ.vec, other.columns[2].vec);
                vW = _mm_mul_ps(vW.vec, other.columns[3].vec);
                vX = _mm_add_ps(vX.vec, vZ.vec);
                vY = _mm_add_ps(vY.vec, vW.vec);
                vX = _mm_add_ps(vX.vec, vY.vec);

                mResult.columns[1] = vX;

                vW = columns[2];
                vX = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(0, 0, 0, 0));
                vY = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(1, 1, 1, 1));
                vZ = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(2, 2, 2, 2));
                vW = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(3, 3, 3, 3));


                vX = _mm_mul_ps(vX.vec, other.columns[0].vec);
                vY = _mm_mul_ps(vY.vec, other.columns[1].vec);
                vZ = _mm_mul_ps(vZ.vec, other.columns[2].vec);
                vW = _mm_mul_ps(vW.vec, other.columns[3].vec);
                vX = _mm_add_ps(vX.vec, vZ.vec);
                vY = _mm_add_ps(vY.vec, vW.vec);
                vX = _mm_add_ps(vX.vec, vY.vec);

                vW = columns[3];
                vX = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(0, 0, 0, 0));
                vY = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(1, 1, 1, 1));
                vZ = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(2, 2, 2, 2));
                vW = XM_PERMUTE_PS(vW.vec, _MM_SHUFFLE(3, 3, 3, 3));


                vX = _mm_mul_ps(vX.vec, other.columns[0].vec);
                vY = _mm_mul_ps(vY.vec, other.columns[1].vec);
                vZ = _mm_mul_ps(vZ.vec, other.columns[2].vec);
                vW = _mm_mul_ps(vW.vec, other.columns[3].vec);
                vX = _mm_add_ps(vX.vec, vZ.vec);
                vY = _mm_add_ps(vY.vec, vW.vec);
                vX = _mm_add_ps(vX.vec, vY.vec);
                mResult.columns[3] = vX;
                return mResult;
            }
            else {
#endif
                Matrix mResult;
                // Cache the invariants in registers
                float x = columns[0][0];
                float y = columns[0][1];
                float z = columns[0][2];
                float w = columns[0][3];
                // Perform the operation on the first row
                mResult.columns[0][0] = (other.columns[0][0] * x) + (other.columns[1][0] * y) + (other.columns[2][0] * z) + (other.columns[3][0] * w);
                mResult.columns[0][1] = (other.columns[0][1] * x) + (other.columns[1][1] * y) + (other.columns[2][1] * z) + (other.columns[3][1] * w);
                mResult.columns[0][2] = (other.columns[0][2] * x) + (other.columns[1][2] * y) + (other.columns[2][2] * z) + (other.columns[3][2] * w);
                mResult.columns[0][3] = (other.columns[0][3] * x) + (other.columns[1][3] * y) + (other.columns[2][3] * z) + (other.columns[3][3] * w);
                // Repeat for all the other rows
                x = columns[1][0];
                y = columns[1][1];
                z = columns[1][2];
                w = columns[1][3];
                mResult.columns[1][0] = (other.columns[0][0] * x) + (other.columns[1][0] * y) + (other.columns[2][0] * z) + (other.columns[3][0] * w);
                mResult.columns[1][1] = (other.columns[0][1] * x) + (other.columns[1][1] * y) + (other.columns[2][1] * z) + (other.columns[3][1] * w);
                mResult.columns[1][2] = (other.columns[0][2] * x) + (other.columns[1][2] * y) + (other.columns[2][2] * z) + (other.columns[3][2] * w);
                mResult.columns[1][3] = (other.columns[0][3] * x) + (other.columns[1][3] * y) + (other.columns[2][3] * z) + (other.columns[3][3] * w);
                x = columns[2][0];
                y = columns[2][1];
                z = columns[2][2];
                w = columns[2][3];
                mResult.columns[2][0] = (other.columns[0][0] * x) + (other.columns[1][0] * y) + (other.columns[2][0] * z) + (other.columns[3][0] * w);
                mResult.columns[2][1] = (other.columns[0][1] * x) + (other.columns[1][1] * y) + (other.columns[2][1] * z) + (other.columns[3][1] * w);
                mResult.columns[2][2] = (other.columns[0][2] * x) + (other.columns[1][2] * y) + (other.columns[2][2] * z) + (other.columns[3][2] * w);
                mResult.columns[2][3] = (other.columns[0][3] * x) + (other.columns[1][3] * y) + (other.columns[2][3] * z) + (other.columns[3][3] * w);
                x = columns[3][0];
                y = columns[3][1];
                z = columns[3][2];
                w = columns[3][3];
                mResult.columns[3][0] = (other.columns[0][0] * x) + (other.columns[1][0] * y) + (other.columns[2][0] * z) + (other.columns[3][0] * w);
                mResult.columns[3][1] = (other.columns[0][1] * x) + (other.columns[1][1] * y) + (other.columns[2][1] * z) + (other.columns[3][1] * w);
                mResult.columns[3][2] = (other.columns[0][2] * x) + (other.columns[1][2] * y) + (other.columns[2][2] * z) + (other.columns[3][2] * w);
                mResult.columns[3][3] = (other.columns[0][3] * x) + (other.columns[1][3] * y) + (other.columns[2][3] * z) + (other.columns[3][3] * w);

                return mResult;
#ifdef USING_SIMD
            }
#endif
#else
#ifdef USING_SIMD
            if constexpr (std::is_constant_evaluated()) {
#endif
                return Matrix{
                    this->operator*(other.columns[0]),
                    this->operator*(other.columns[1]),
                    this->operator*(other.columns[2]),
                    this->operator*(other.columns[3])
                };
#ifdef USING_SIMD
            }
            else {

                Matrix ret;
                for(uint8_t i = 0; i < 4; i++){
                    const __m128 v[4] = {
                        XM_PERMUTE_PS(columns[i].vec.vec, _MM_SHUFFLE(0, 0, 0, 0)),
                        XM_PERMUTE_PS(columns[i].vec.vec, _MM_SHUFFLE(1, 1, 1, 1)),
                        XM_PERMUTE_PS(columns[i].vec.vec, _MM_SHUFFLE(2, 2, 2, 2)),
                        XM_PERMUTE_PS(columns[i].vec.vec, _MM_SHUFFLE(3, 3, 3, 3)),
                    };
                    ret.columns[i].vec.vec = _mm_add_ps(_mm_add_ps(v[0], v[2]), _mm_add_ps(v[1], v[3]));
                }
/*
                for (uint8_t i = 0; i < 4; ++i) {
                    //the matrix * vector operator isnt good here, im assuming because it converts from __m128 to vector to __m128 or something. idk. couldve been a benchmark error
                    const __m128 mul0 = _mm_mul_ps(columns[0].vec, _mm_set1_ps(other.columns[i].x));
                    const __m128 mul1 = _mm_mul_ps(columns[1].vec, _mm_set1_ps(other.columns[i].y));
                    const __m128 mul2 = _mm_mul_ps(columns[2].vec, _mm_set1_ps(other.columns[i].z));
                    const __m128 mul3 = _mm_mul_ps(columns[3].vec, _mm_set1_ps(other.columns[i].w));

                    ret.columns[i].vec = _mm_add_ps(_mm_add_ps(mul0, mul1), _mm_add_ps(mul2, mul3));
                }
                */
                return ret;
            }
#endif
#endif
        }

        LAB_constexpr Matrix& operator*=(Matrix<F, 4, 4, 4> const& other) {
            //its necessary to make a copy unfortunately
            //there might be some shenanigans where i can avoid it, but i suspect not
            return *this = operator*(other);
        }

        LAB_constexpr Matrix& operator*=(F const multiplier) {
            for (uint8_t column = 0; column < 4; column++) {
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
            for (uint8_t column = 0; column < 4; column++) {
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
            for (uint8_t column = 0; column < 4; column++) {
                for (uint8_t row = 0; row < 4; row++) {
                    ret.columns[row][column] = columns[column][row];
                }
            }
            return ret;
        }

        //for the normal matrix, a 3x3 matrix is extracted, and transpose(inverse(3x3))
        LAB_constexpr F GetDeterminant() const {
            F const SubFactor00 = columns[2][2] * columns[3][3] - columns[3][2] * columns[2][3];
            F const SubFactor01 = columns[2][1] * columns[3][3] - columns[3][1] * columns[2][3];
            F const SubFactor02 = columns[2][1] * columns[3][2] - columns[3][1] * columns[2][2];
            F const SubFactor03 = columns[2][0] * columns[3][3] - columns[3][0] * columns[2][3];
            F const SubFactor04 = columns[2][0] * columns[3][2] - columns[3][0] * columns[2][2];
            F const SubFactor05 = columns[2][0] * columns[3][1] - columns[3][0] * columns[2][1];

            Vector<F, 4> DetCoeff(
                +(columns[1][1] * SubFactor00 - columns[1][2] * SubFactor01 + columns[1][3] * SubFactor02),
                -(columns[1][0] * SubFactor00 - columns[1][2] * SubFactor03 + columns[1][3] * SubFactor04),
                +(columns[1][0] * SubFactor01 - columns[1][1] * SubFactor03 + columns[1][3] * SubFactor05),
                -(columns[1][0] * SubFactor02 - columns[1][1] * SubFactor04 + columns[1][2] * SubFactor05)
            );

            return
                columns[0][0] * DetCoeff[0] + columns[0][1] * DetCoeff[1] +
                columns[0][2] * DetCoeff[2] + columns[0][3] * DetCoeff[3];
        }
        LAB_constexpr Matrix GetInverse() const {
            Matrix inv{F(0)};

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

//wrap this in a debug expression?
#if LAB_DEBUGGING_FLOAT_ANOMALIES
            //if (lab::Abs(det) < 1e-6f)
#endif
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
    LAB_constexpr Matrix<F, 4, 4> IdentityTranslation(Vector<F, 3> const& translation) {
        return Matrix<F, 4, 4, 4>{
            Vector<F, 4>(F(1), F(0), F(0), F(0)),
            Vector<F, 4>(F(0), F(1), F(0), F(0)),
            Vector<F, 4>(F(0), F(0), F(1), F(0)),
            Vector<F, 4>(translation, F(1)),
        };
    }
    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> IdentityTranslation(F const x, F const y, F const z) {
        return IdentityTranslation(Vector<F, 3>{x, y, z});
    }

    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> IdentityScale(Vector<F, 3> const& scale){
        return Matrix<F, 4, 4, 4>{
            Vector<F, 4>(scale.x, F(0), F(0), F(0)),
            Vector<F, 4>(F(0), scale.y, F(0), F(0)),
            Vector<F, 4>(F(0), F(0), scale.z, F(0)),
            Vector<F, 4>(F(0), F(0), F(0), F(1)),
        };
    }
    template<std::floating_point F>
    LAB_constexpr Matrix<F, 4, 4> IdentityScale(F const x, F const y, F const z) {
        return IdentityScale(Vector<F, 3>{x, y, z});
    }

    template<std::floating_point F>
    Matrix<float, 4, 4> RotateAroundAxis(F const angle, Vector<F, 3> const axis) {

        const F magSq = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
        if (magSq == F(0)) {
            return Matrix<F, 4, 4>{};
        }
        const F invLen = F(1) / Sqrt(magSq);
        const F x = axis.x * invLen;
        const F y = axis.y * invLen;
        const F z = axis.z * invLen;

        const F c = Cos(angle);
        const F s = Sin(angle);
        const F t = F(1) - c;

        Matrix<F, 4, 4> m{};
        m.At(0, 0) = t*x*x + c;
        m.At(0, 1) = t*x*y + s*z;
        m.At(0, 2) = t*x*z - s*y;
        m.At(0, 3) = F(0);

        m.At(1, 0) = t*x*y - s*z;
        m.At(1, 1) = t*y*y + c;
        m.At(1, 2) = t*y*z + s*x;
        m.At(1, 3) = F(0);

        m.At(2, 0) = t*x*z + s*y;
        m.At(2, 1) = t*y*z - s*x;
        m.At(2, 2) = t*z*z + c;
        m.At(2, 3) = F(0);

        m.At(3, 0) = F(0);
        m.At(3, 1) = F(0);
        m.At(3, 2) = F(0);
        m.At(3, 3) = F(1);

        return m;
    }

    template<std::floating_point F>
    Matrix<F, 4, 4> RotateAroundX(F const angle) {
        F const cosine = Cos(angle);       
#ifdef LAB_LEFT_HANDED
        F const sine = -Sin(angle);
#else
        F const sine = Sin(angle);
#endif
    
        return Matrix<F, 4, 4>(
            Vector<F, 4>(F(1), F(0), F(0), F(0)),
            Vector<F, 4>(F(0),  cosine, -sine, F(0)),
            Vector<F, 4>(F(0), sine, cosine, F(0)),
            Vector<F, 4>(F(0), F(0), F(0), F(1))
        );
    }

    template<std::floating_point F>
    Matrix<F, 4, 4> RotateAroundY(F const angle) {
        F const cosine = Cos(angle);       
#ifdef LAB_LEFT_HANDED
        F const sine = -Sin(angle);
#else
        F const sine = Sin(angle);
#endif
    
        return Matrix<F, 4, 4>(
            Vector<F, 4>(cosine, F(0), sine, F(0)),
            Vector<F, 4>(F(0), F(1), F(0), F(0)),
            Vector<F, 4>(-sine, F(0), cosine, F(0)),
            Vector<F, 4>(F(0), F(0), F(0), F(1))
        );
    }
    template<std::floating_point F>
    Matrix<F, 4, 4> RotateAroundZ(F const angle) {
        F const cosine = Cos(angle);        
#ifdef LAB_LEFT_HANDED
        F const sine = -Sin(angle);
#else
        F const sine = Sin(angle);
#endif
    
        return Matrix<F, 4, 4>(
            Vector<F, 4>(cosine, -sine, F(0), F(0)),
            Vector<F, 4>(sine, cosine, F(0), F(0)),
            Vector<F, 4>(F(0), F(0), F(1), F(0)),
            Vector<F, 4>(F(0), F(0), F(0), F(1))
        );
    }


#ifdef LAB_ROW_MAJOR
    LAB_constexpr Vector<float, 4> operator*(Vector<float, 4> const& vector, Matrix<float, 4, 4, 4> const& matrix) {
        //copied from dxm
#ifdef USING_SIMD
        if constexpr (!std::is_constant_evaluated()) {

            __m128 vResult = XM_PERMUTE_PS(vector.ToSIMD(), _MM_SHUFFLE(3, 3, 3, 3)); // W
            vResult = _mm_mul_ps(vResult, matrix.columns[3].vec);
            __m128 vTemp = XM_PERMUTE_PS(vector.ToSIMD(), _MM_SHUFFLE(2, 2, 2, 2)); // Z
            vResult = XM_FMADD_PS(vTemp, matrix.columns[2].vec, vResult);
            vTemp = XM_PERMUTE_PS(vector.ToSIMD(), _MM_SHUFFLE(1, 1, 1, 1)); // Y
            vResult = XM_FMADD_PS(vTemp, matrix.columns[1].vec, vResult);
            vTemp = XM_PERMUTE_PS(vector.ToSIMD(), _MM_SHUFFLE(0, 0, 0, 0)); // X
            vResult = XM_FMADD_PS(vTemp, matrix.columns[0].vec, vResult);

            return Vector<float, 4>{vResult};
        }
        else {
#endif
            auto temp = matrix.columns[0];
            auto secondTemp = temp[0];
            return Vector<float, 4>{
                (matrix.columns[0][0] * vector.x) + (matrix.columns[1][0] * vector.y) + (matrix.columns[2][0] * vector.z) + (matrix.columns[3][0] * vector.w),
                (matrix.columns[0][1] * vector.x) + (matrix.columns[1][1] * vector.y) + (matrix.columns[2][1] * vector.z) + (matrix.columns[3][1] * vector.w),
                (matrix.columns[0][2] * vector.x) + (matrix.columns[1][2] * vector.y) + (matrix.columns[2][2] * vector.z) + (matrix.columns[3][2] * vector.w),
                (matrix.columns[0][3] * vector.x) + (matrix.columns[1][3] * vector.y) + (matrix.columns[2][3] * vector.z) + (matrix.columns[3][3] * vector.w)
            };
#ifdef USING_SIMD
        }
#endif
    }
#endif

}//namespace Linear_Algebra


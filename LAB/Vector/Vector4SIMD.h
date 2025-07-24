#pragma once

#if defined(LAB_USING_SSE) || (LAB_USING_AVX2)
#define USING_SIMD
#include <immintrin.h>
#endif

#ifdef USING_SIMD

//im not going to make a double version of this yet

namespace lab{
    struct VectorSIMD {
        __m128 vec;
        //simd cant be constexpr (currently)
        VectorSIMD() : vec{} {}
        VectorSIMD(__m128 const& vec) : vec{vec} {}

        VectorSIMD& operator=(VectorSIMD const& other){
            vec = other.vec;
            return *this;
        }
        VectorSIMD operator-() const{
            return VectorSIMD{
                _mm_xor_ps(vec, _mm_set_ps1(-0.f))
            };
        }
        bool operator==(VectorSIMD const other) const {
            return _mm_movemask_ps(_mm_cmpeq_ps(vec, other.vec)) == 0xF;
        }
        VectorSIMD operator+(VectorSIMD const other) const {
            return VectorSIMD(_mm_add_ps(vec, other.vec));
        }
        VectorSIMD& operator+=(VectorSIMD const other) {
            vec = _mm_add_ps(vec, other.vec);
            return *this;
        }

        VectorSIMD operator*(VectorSIMD const other) const {
            return VectorSIMD(_mm_mul_ps(vec, other.vec));
        }
        VectorSIMD& operator*=(VectorSIMD const other) {
            vec = _mm_mul_ps(vec, other.vec);
            return *this;
        }
        
        VectorSIMD operator*(float const multiplier) const {
            return VectorSIMD{_mm_mul_ps(vec, _mm_set_ps1(multiplier)) };
        }
        VectorSIMD& operator*=(float const multiplier) {
            vec = _mm_mul_ps(vec, _mm_set_ps1(multiplier));
            return *this;
        }

        VectorSIMD& operator-=(VectorSIMD const other) {
            vec = _mm_sub_ps(vec, other.vec);
            return *this;
        }
        VectorSIMD operator-(VectorSIMD const other) const {
            return VectorSIMD{_mm_sub_ps(vec, other.vec)};
        }

        VectorSIMD& operator/=(float const divisor) {
            vec = _mm_div_ps(vec, _mm_set_ps1(divisor));
            return *this;
        }
        VectorSIMD operator/(float const divisor) const {
            return VectorSIMD{_mm_div_ps(vec, _mm_set_ps1(divisor))};
        }

    };
}
#endif
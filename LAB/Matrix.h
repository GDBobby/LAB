#pragma once
#include "Matrix/MatrixTxT.h"
#include "Matrix/Matrix2x2.h"
#include "Matrix/Matrix3x3.h"
#include "Matrix/Matrix4x4.h"


namespace lab{
    //operators here


	using mat2 = Matrix<float, 2, 2>;
	using mat3 = Matrix<float, 3, 3, 4>;
	using mat4 = Matrix<float, 4, 4>;
	using mat2f = Matrix<float, 2, 2>;
	using mat3f = Matrix<float, 3, 3, 4>;
	using mat4f = Matrix<float, 4, 4>;
	using mat2d = Matrix<double, 2, 2>;
	using mat3d = Matrix<double, 3, 3>; //9 * 8 == 72, should be aligned for GLSL. idk. might still need the 4 alignment
	using mat4d = Matrix<double, 4, 4>;
}
#include "Vector.h"
#include "Matrix.h"

#include <cstdio>

int main() {
	constexpr Linear_Algebra::Vector<float, 2> checkVec1(1.f, 2.f);
	constexpr Linear_Algebra::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
	static_assert((checkVec1 / 2.f) == checkVec2);

	constexpr Linear_Algebra::Vector<float, 2> checkVec3(3.f, 4.f);

	constexpr auto checkMat = Linear_Algebra::CreateMatrix(checkVec1, checkVec3);

	static_assert(checkMat.At(1, 1) == checkVec3.y);

	Linear_Algebra::Vector<float, 2> myVec{ 1.f, 2.f };
	printf("myVec sqrMag result : %.2f\n", myVec.SquaredMagnitude());
	myVec/= 2.f;
	printf("myVec /= result - %.2f:%.2f\n", myVec.x, myVec.y);

}
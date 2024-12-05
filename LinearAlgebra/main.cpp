#include "Vector.h"

#include <cstdio>

int main() {
	Linear_Algebra::Vector<float, 2> myVec{1.f, 2.f};

	

	printf("myVec sqrMag result : %.2f\n", myVec.SquaredMagnitude());

	myVec/= 2.f;

	printf("myVec /= result - %.2f:%.2f\n", myVec.x, myVec.y);

	//need an == operator
	static_assert((Linear_Algebra::Vector<float, 2>(1.f, 2.f)/2.f).x == 1.f / 2.f);

}
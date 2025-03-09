#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"

#include <cstdio>

int main() {
	constexpr Linear_Algebra::Vector<float, 2> checkVec1(1.f, 2.f);
	constexpr Linear_Algebra::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
	static_assert((checkVec1 / 2.f) == checkVec2);

	constexpr Linear_Algebra::Vector<float, 2> checkVec3(3.f, 4.f);

	constexpr auto checkMat = Linear_Algebra::CreateMatrix<float, 2, 8>(checkVec1, checkVec3);

	static_assert(checkMat.At(1, 1) == checkVec3.y);

	Linear_Algebra::Vector<float, 2> myVec{ 1.f, 2.f };
	printf("myVec sqrMag result : %.2f\n", myVec.SquaredMagnitude());
	myVec/= 2.f;
	printf("myVec /= result - %.2f:%.2f\n", myVec.x, myVec.y);

	{
		constexpr Linear_Algebra::Transform<float, 2> transform{};

		constexpr auto mat = transform.ToMatrix<12>();
		static_assert(transform.translation.x == mat.At(0, 0));
		static_assert(transform.translation.y == mat.At(0, 1));
		static_assert(transform.scale.x == mat.At(1, 0));
		static_assert(transform.scale.y == mat.At(1, 1));
		static_assert(transform.rotation == mat.At(2, 0));
	}
	{
		constexpr Linear_Algebra::Transform<float, 2> transform{Linear_Algebra::Vector<float, 2>{1.f, 0.f}, Linear_Algebra::Vector<float, 2>{1.1f, 1.2f}, 0.f};

		constexpr auto mat = transform.ToMatrix<12>();
		static_assert(transform.translation.x == mat.At(0, 0));
		static_assert(transform.translation.y == mat.At(0, 1));
		static_assert(transform.scale.x == mat.At(1, 0));
		static_assert(transform.scale.y == mat.At(1, 1));
		static_assert(transform.rotation == mat.At(2, 0));
	}
	printf("made it to the end\n");
}
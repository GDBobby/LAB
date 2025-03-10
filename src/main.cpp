#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"

#include <cstdio>
#include <fstream>

int main() {
	{
		constexpr Linear_Algebra::Vector<float, 2> checkVec1(1.f, 2.f);
		constexpr Linear_Algebra::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
		static_assert((checkVec1 / 2.f) == checkVec2);

		constexpr Linear_Algebra::Vector<float, 2> checkVec3(3.f, 4.f);

		constexpr auto checkMat = Linear_Algebra::CreateMatrix<float, 2, 8>(checkVec1, checkVec3);

		static_assert(checkMat.At(1, 1) == checkVec3.y);

		Linear_Algebra::Vector<float, 2> myVec{ 1.f, 2.f };
		//printf("myVec sqrMag result : %.2f\n", myVec.SquaredMagnitude());
		myVec /= 2.f;
		//printf("myVec /= result - %.2f:%.2f\n", myVec.x, myVec.y);
	}
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
	{
		constexpr Linear_Algebra::Transform<float, 2> transform{Linear_Algebra::Vector<float, 2>{1.f, 0.f}, Linear_Algebra::Vector<float, 2>{1.1f, 1.2f}, 0.f};

		constexpr auto mat = transform.ToMatrix<16>();
		static_assert(transform.translation.x == mat.At(0, 0));
		static_assert(transform.translation.y == mat.At(0, 1));
		static_assert(transform.scale.x == mat.At(1, 0));
		static_assert(transform.scale.y == mat.At(1, 1));
		static_assert(transform.rotation == mat.At(2, 0));
	}

	{
		
		constexpr auto mat1 = Linear_Algebra::CreateMatrix<float, 3, 16>(Linear_Algebra::Vector<float, 3>(0.f, 1.f, 2.f), Linear_Algebra::Vector<float, 3>(2.f, 3.f, 4.f), Linear_Algebra::Vector<float, 3>(3.f, 4.f, 5.f));
		constexpr auto mat2 = Linear_Algebra::CreateMatrix<float, 3, 12>(Linear_Algebra::Vector<float, 3>(10.f, 11.f, 12.f), Linear_Algebra::Vector<float, 3>(22.f, 23.f, 24.f), Linear_Algebra::Vector<float, 3>(33.f, 34.f, 35.f));

		static_assert(mat1.At(0, 0) == 0.f);
		constexpr auto intermediate = mat1 * mat2;
		constexpr auto intermediateReverse = mat2 * mat1;
		static_assert(intermediate.At(0, 0) != intermediateReverse.At(0, 0));

#ifdef _WIN32
		std::ofstream outFile{ "windows_output.txt", std::ios::binary };
#elif __unix__
		std::ofstream outFile{ "linux_output.txt", std::ios::binary };
#endif
		if (!outFile.is_open()) {
			return EXIT_FAILURE;
		}
		outFile.write(reinterpret_cast<const char*>(intermediate.data), sizeof(float) * 12);
		outFile.write(reinterpret_cast<const char*>(intermediateReverse.data), sizeof(float) * 9);
		
		outFile.close();
	}
	printf("made it to the end\n");
	return EXIT_SUCCESS;
}
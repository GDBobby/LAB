#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"

#include <cstdio>
#include <fstream>

//#define STATIC_ASSERT_DEBUGGING

#ifdef STATIC_ASSERT_DEBUGGING
	#define LA_static_assert assert
#else
	#define LA_static_assert static_assert
#endif

int main() {
	{
		constexpr Linear_Algebra::Vector<float, 2> checkVec1(1.f, 2.f);
		constexpr Linear_Algebra::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
		static_assert((checkVec1 / 2.f) == checkVec2);

		constexpr Linear_Algebra::Vector<float, 2> checkVec3(3.f, 4.f);

		constexpr auto checkMat = Linear_Algebra::CreateMatrix<float, 2, 8>(checkVec1, checkVec3);

		LA_static_assert(checkMat.At(1, 1) == checkVec3.y);

		Linear_Algebra::Vector<float, 2> myVec{ 1.f, 2.f };
		myVec /= 2.f;
	}

	{
		constexpr auto mat1 = Linear_Algebra::CreateMatrix<float, 3, 16>(Linear_Algebra::Vector<float, 3>(0.f, 1.f, 2.f), Linear_Algebra::Vector<float, 3>(2.f, 3.f, 4.f), Linear_Algebra::Vector<float, 3>(3.f, 4.f, 5.f));
		constexpr auto mat2 = Linear_Algebra::CreateMatrix<float, 3, 12>(Linear_Algebra::Vector<float, 3>(10.f, 11.f, 12.f), Linear_Algebra::Vector<float, 3>(22.f, 23.f, 24.f), Linear_Algebra::Vector<float, 3>(33.f, 34.f, 35.f));

		LA_static_assert(mat1.At(0, 0) == 0.f);
		constexpr auto intermediate = mat1 * mat2;
		constexpr auto intermediateReverse = mat2 * mat1;
		LA_static_assert(intermediate.At(0, 0) != intermediateReverse.At(0, 0));
#ifdef _MSC_VER
		std::ofstream outFile{ "msvc_output.txt", std::ios::binary };
#elif defined(__clang__)
		std::ofstream outFile{ "clang_output.txt", std::ios::binary };
#elif defined(__GNUC__)
		std::ofstream outFile{ "gcc_output.txt", std::ios::binary };
#endif
		if (!outFile.is_open()) {
			return EXIT_FAILURE;
		}
		for(uint8_t x = 0; x < 3; x++){
			for(uint8_t y = 0; y < 3; y++){
				const float first = intermediate.At(x, y);
				const float second = intermediateReverse.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&first), sizeof(float));
				outFile.write(reinterpret_cast<const char*>(&second), sizeof(float));
			}
		}
		
		outFile.close();
	}
	printf("made it to the end\n");
	return EXIT_SUCCESS;
}
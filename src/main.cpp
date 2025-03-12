#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"
#include "Camera.h"

#include <cstdio>
#include <fstream>
#include <concepts>
#include <type_traits>

#include <cmath>

//#define STATIC_ASSERT_DEBUGGING

#ifdef STATIC_ASSERT_DEBUGGING
	#define LA_static_assert assert
#else
	#define LA_static_assert static_assert
#endif




int main() {
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

	{
		constexpr LAB::Vector<float, 2> checkVec1(1.f, 2.f);
		constexpr LAB::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
		LA_static_assert((checkVec1 / 2.f) == checkVec2);

		constexpr LAB::Vector<float, 2> checkVec3(3.f, 4.f);

		constexpr auto checkMat = LAB::CreateMatrix<float, 2, 8>(checkVec1, checkVec3);

		LA_static_assert(checkMat.At(1, 1) == checkVec3.y);

		LAB::Vector<float, 2> myVec{ 1.f, 2.f };
		myVec /= 2.f;
		constexpr float nDP = LAB::NormalizedDotProduct(checkVec1, checkVec2);

		outFile.write(reinterpret_cast<const char*>(&nDP), sizeof(float));
		//printf("nDP : %.2f\n", nDP);

		static_assert(LAB::Vector<float, 2>::Forward().x == LAB::Vector<float, 2>::Up().y);
	}

	{
		constexpr auto mat1 = LAB::CreateMatrix<float, 3, 16>(LAB::Vector<float, 3>(0.f, 1.f, 2.f), LAB::Vector<float, 3>(2.f, 3.f, 4.f), LAB::Vector<float, 3>(3.f, 4.f, 5.f));
		constexpr auto mat2 = LAB::CreateMatrix<float, 3, 12>(LAB::Vector<float, 3>(10.f, 11.f, 12.f), LAB::Vector<float, 3>(22.f, 23.f, 24.f), LAB::Vector<float, 3>(33.f, 34.f, 35.f));

		LA_static_assert(mat1.At(0, 0) == 0.f);
		constexpr auto intermediate = mat1 * mat2;
		constexpr auto intermediateReverse = mat2 * mat1;
		LA_static_assert(intermediate.At(0, 0) != intermediateReverse.At(0, 0));

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
	{
		constexpr LAB::Transform<float, 3> transform{};
		constexpr auto scaleMat = transform.GetScaleMatrix<16>();
		constexpr LAB::Matrix<float, 4, 4, 16> identityMat{LAB::Matrix<float, 4, 4, 16>::Identity(1.f)};
		LA_static_assert(scaleMat.data[0] == 1.f);
		LA_static_assert(identityMat.data[0] == 1.f);

		LA_static_assert(scaleMat == identityMat);
	}
	{
		constexpr LAB::Transform<float, 3> transformX{LAB::Vector<float, 3>{0.f}, LAB::Vector<float, 3>{1.f}, LAB::Vector<float, 3>{std::numbers::pi_v<float>, 0.f, 0.f}};
		constexpr LAB::Transform<float, 3> transformY{LAB::Vector<float, 3>{0.f}, LAB::Vector<float, 3>{1.f}, LAB::Vector<float, 3>{0.f, std::numbers::pi_v<float>, 0.f}};
		constexpr LAB::Transform<float, 3> transformZ{LAB::Vector<float, 3>{0.f}, LAB::Vector<float, 3>{1.f}, LAB::Vector<float, 3>{0.f, 0.f, std::numbers::pi_v<float>}};

		constexpr auto rotXMat = transformX.GetRotationXMatrix();
		constexpr auto rotYMat = transformY.GetRotationYMatrix();
		constexpr auto rotZMat = transformZ.GetRotationZMatrix();

		LA_static_assert(rotXMat.At(1, 1) == rotYMat.At(0, 0));
		LA_static_assert(rotXMat.At(2, 1) == rotYMat.At(0, 2));
		LA_static_assert(rotXMat.At(1, 2) == rotYMat.At(2, 0));
		LA_static_assert(rotXMat.At(2, 2) == rotYMat.At(2, 2));

		LA_static_assert(rotZMat.At(0, 0) == rotYMat.At(0, 0));
		LA_static_assert(rotZMat.At(1, 0) == rotYMat.At(0, 2));
		LA_static_assert(rotZMat.At(0, 1) == rotYMat.At(2, 0));
		LA_static_assert(rotZMat.At(1, 1) == rotYMat.At(2, 2));

		for(uint8_t x = 0; x < 3; x++){
			for(uint8_t y = 0; y < 3; y++){
				const float xval = rotXMat.At(x, y);
				const float yval = rotYMat.At(x, y);
				const float zval = rotZMat.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&xval), sizeof(float));
				outFile.write(reinterpret_cast<const char*>(&yval), sizeof(float));
				outFile.write(reinterpret_cast<const char*>(&zval), sizeof(float));
			}
		}
	}
	{
		constexpr auto proj = LAB::CreateProjectionMatrix<float>(LAB::SupportingMath::DegreesToRadians(70.f), 0.f, 100.f);
	
		for(uint8_t x = 0; x < 4; x++){
			for(uint8_t y = 0; y < 4; y++){
				const float tempVal = proj.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&tempVal), sizeof(float));
			}
		}
	}
	{
		constexpr float trigInput = 50.f;
		constexpr auto cosRet = LAB::SupportingMath::Cos(trigInput);
		constexpr auto sinRet = LAB::SupportingMath::Sin(trigInput);
		constexpr auto tanRet = LAB::SupportingMath::Tan(trigInput);

		outFile.write(reinterpret_cast<const char*>(&cosRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&sinRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&tanRet), sizeof(float));

		printf("cos comparison : (%.10f) - (%.10f)\n", cosRet, std::cos(trigInput));
		printf("sin comparison : (%.10f) - (%.10f)\n", sinRet, std::sin(trigInput));
		printf("tan comparison : (%.10f) - (%.10f)\n", tanRet, std::tan(trigInput));
	}
	{
		constexpr auto viewMat = LAB::CreateViewMatrix(LAB::Vector<float, 3>(0.f), LAB::Vector<float, 3>::Forward());
	}

	printf("made it to the end\n");
	return EXIT_SUCCESS;
}
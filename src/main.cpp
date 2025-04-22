#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"
#include "Camera.h"

#include <cstdio>
#include <fstream>
#include <concepts>
#include <type_traits>

#include <cmath>

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
		LAB_constexpr LAB::Matrix<float, 4, 4> test1{0.f};
		LAB_constexpr LAB::Matrix<float, 4, 4> test2{1.f};

		LAB_constexpr LAB::Matrix<float, 4 ,4> test3 = test1 * test2;
		LAB_constexpr LAB::Vector<float, 4> test4 = test1 * LAB::Vector<float, 4>{1.f};
		printf("test mat multiplpication print : %.2f - %.2f\n", test3.columns[0][0], test4.x);

		LAB::Matrix<float, 4, 4> crossTest{1.f};
		LAB::Matrix<float, 4, 4> crossTest2{2.f};
		LAB::Matrix<float, 4, 4> crossTestOut = crossTest * crossTest2;
		outFile.write(reinterpret_cast<const char*>(&crossTestOut.columns), sizeof(float) * 4 * 4);
	}

	{ //vectors
		LAB_constexpr LAB::Vector<float, 3> vec3a(1.0f, 2.0f, 3.0f);
		LAB_constexpr LAB::Vector<float, 3> vec3b(3.0f, 4.0f, 5.0f);
		LAB_constexpr LAB::Vector<float, 3> vec3c(5.0f, 6.0f, 7.0f);
		LAB_constexpr LAB::Vector<float, 3> vec3d(7.0f, 8.0f, 9.0f);
		LAB::CrossProduct((vec3a + vec3b) * vec3c - vec3d, vec3a);
		LAB_constexpr LAB::Vector<float, 3> vec3aN = vec3a.Normalized();
		LAB_constexpr LAB::Vector<float, 3> vec3bN = vec3b.Normalized();
		LAB_constexpr LAB::Vector<float, 3> vec3cN = vec3c.Normalized();
		LAB_constexpr LAB::Vector<float, 3> vec3dN = vec3d.Normalized();
		printf("normalized dots - %.2f\n", vec3aN.DotProduct(vec3bN) + vec3cN.DotProduct(vec3dN));

		LAB_constexpr LAB::Matrix<float, 3, 2> imbalancedMat(0.f);
		printf("imbalanced mat usage : %.2f\n", imbalancedMat.At(2, 1));

		LAB::Vector<float, 2> myVec{ 1.f, 2.f };
		myVec /= 2.f;
		LAB_constexpr float nDP = LAB::NormalizedDotProduct(vec3a, vec3b);

		outFile.write(reinterpret_cast<const char*>(&nDP), sizeof(float));
		//printf("nDP : %.2f\n", nDP);

		LAB_constexpr float forwardX = LAB::Vector<float, 2>::Forward().x;
		LAB_constexpr float upY = LAB::Vector<float, 2>::Up().y;
		printf("unit vector - %.2f:%.2f\n", forwardX, upY);

		LAB_static_assert(forwardX == upY);
	}

	{ //matrices

		LAB_constexpr LAB::Vector<float, 2> checkVec1(1.f, 2.f);
		LAB_constexpr LAB::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
		LAB_static_assert((checkVec1 / 2.f) == checkVec2);

		LAB_constexpr LAB::Vector<float, 2> checkVec3(3.f, 4.f);

		LAB_constexpr LAB::Matrix<float, 2, 2> checkMat{{checkVec1, checkVec3 }};
		LAB_static_assert(checkMat.At(1, 1) == checkVec3.y);
		printf("chekc mat print : %.2f\n", checkMat.At(1, 1));
		//LAB_constexpr std::array<LAB::Vector<float, 3>, 3> vecArray{ LAB::Vector<float, 3>(0.f, 1.f, 2.f), LAB::Vector<float, 3>(2.f, 3.f, 4.f), LAB::Vector<float, 3>(3.f, 4.f, 5.f) };
		//LAB::Matrix<float, 3, 3> checkMatNonConst(0.f);
		//checkMatNonConst.columns[0] = vecArray[0];

		//LAB_constexpr LAB::Matrix<float, 3, 3, 4> mat1(vecArray);
		/*
		LAB_constexpr LAB::Matrix<float, 3, 3> mat2{ LAB::Vector<float, 3>(10.f, 11.f, 12.f), LAB::Vector<float, 3>(22.f, 23.f, 24.f), LAB::Vector<float, 3>(33.f, 34.f, 35.f) };

		LAB_static_assert(mat1.At(0, 0) == 0.f);
		LAB_constexpr auto intermediate = mat1 * mat2;
		LAB_constexpr auto intermediateReverse = mat2 * mat1;
		LAB_static_assert(intermediate.At(0, 0) != intermediateReverse.At(0, 0));

		for(uint8_t x = 0; x < 3; x++){
			for(uint8_t y = 0; y < 3; y++){
				const float first = intermediate.At(x, y);
				const float second = intermediateReverse.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&first), sizeof(float));
				outFile.write(reinterpret_cast<const char*>(&second), sizeof(float));
			}
		}
		*/
		
		outFile.close();
	}
	{ //rotation, scale, and translate of matrices
		LAB_constexpr LAB::Vector<float, 3> testVec{ 0.f, 1.f, 2.f };
		LAB_constexpr float testFloat0 = testVec[0];
		LAB_constexpr float testFloat1 = testVec[1];
		LAB_static_assert(testFloat0 != testFloat1);

		LAB_constexpr LAB::Matrix<float, 4, 4> matrix1{ 1.f };
		LAB_constexpr auto rotatedMat = LAB::Rotate(matrix1, 0.5f, LAB::Vector<float, 3>::Up());

		LAB_constexpr auto translatedMat = LAB::Translate(rotatedMat, LAB::Vector<float, 3>{0.f, 1.f, 2.f});
		LAB_constexpr auto scaledMat = LAB::Scale(translatedMat, testVec);
		LAB_static_assert(scaledMat.At(0, 0) != scaledMat.At(3, 2));
		printf("scaledMat 0 - %.2f\n", scaledMat.At(0, 0));

		LAB_constexpr LAB::Vector<float, 4> preMultVec{ 1.f };
		LAB_constexpr LAB::Vector<float, 4> postMultVec = scaledMat * preMultVec;
		printf("postMultVec usage : %.2f\n", postMultVec.x);
	}
	{
	}
	{ //rotation matrices, scale matrix
		LAB_constexpr LAB::Transform<float, 3> transformX{LAB::Vector<float, 3>{0.f}, LAB::Vector<float, 3>{1.f}, LAB::Vector<float, 3>{LAB::SupportingMath::PI<float>, 0.f, 0.f}};
		LAB_constexpr LAB::Transform<float, 3> transformY{LAB::Vector<float, 3>{0.f}, LAB::Vector<float, 3>{1.f}, LAB::Vector<float, 3>{0.f, LAB::SupportingMath::PI<float>, 0.f}};
		LAB_constexpr LAB::Transform<float, 3> transformZ{LAB::Vector<float, 3>{0.f}, LAB::Vector<float, 3>{1.f}, LAB::Vector<float, 3>{0.f, 0.f, LAB::SupportingMath::PI<float>}};

		LAB_constexpr auto rotXMat = transformX.GetRotationXMatrix();
		LAB_constexpr auto rotYMat = transformY.GetRotationYMatrix();
		LAB_constexpr auto rotZMat = transformZ.GetRotationZMatrix();

		LAB_static_assert(rotXMat.At(1, 1) == rotYMat.At(0, 0));
		LAB_static_assert(rotXMat.At(2, 1) == rotYMat.At(0, 2));
		LAB_static_assert(rotXMat.At(1, 2) == rotYMat.At(2, 0));
		LAB_static_assert(rotXMat.At(2, 2) == rotYMat.At(2, 2));

		LAB_static_assert(rotZMat.At(0, 0) == rotYMat.At(0, 0));
		LAB_static_assert(rotZMat.At(1, 0) == rotYMat.At(0, 2));
		LAB_static_assert(rotZMat.At(0, 1) == rotYMat.At(2, 0));
		LAB_static_assert(rotZMat.At(1, 1) == rotYMat.At(2, 2));

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
		
		LAB_constexpr LAB::Transform<float, 3> transformScale{};
		LAB_constexpr auto scaleMat = transformScale.GetScaleMatrix();
		LAB_constexpr LAB::Matrix<float, 4, 4> identityMat{LAB::Identity<float, 4>(1.f)};
		LAB_static_assert(scaleMat.columns[0][0] == 1.f);
		LAB_static_assert(identityMat.columns[0][0] == 1.f);

		LAB_static_assert(scaleMat == identityMat);
		printf("scale and identity - %.2f:%.2f\n", scaleMat.columns[0][0], identityMat.columns[0][0]);
	}
	{ //camera functions
		LAB_constexpr auto proj = LAB::CreateProjectionMatrix<float>(LAB::SupportingMath::DegreesToRadians(70.f), 1.44f, 0.f, 100.f);
	
		for(uint8_t x = 0; x < 4; x++){
			for(uint8_t y = 0; y < 4; y++){
				const float tempVal = proj.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&tempVal), sizeof(float));
			}
		}
		LAB_constexpr auto viewMat = LAB::CreateViewMatrix(LAB::Vector<float, 3>(0.f), LAB::Vector<float, 3>::Forward());
		for (uint8_t x = 0; x < 4; x++) {
			for (uint8_t y = 0; y < 4; y++) {
				const float tempVal = viewMat.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&tempVal), sizeof(float));
			}
		}
	}
	{ //trig functions
		LAB_constexpr float trigInput = 50.f;
		LAB_constexpr auto cosRet = LAB::SupportingMath::Cos(trigInput);
		LAB_constexpr auto sinRet = LAB::SupportingMath::Sin(trigInput);
		LAB_constexpr auto tanRet = LAB::SupportingMath::Tan(trigInput);

		outFile.write(reinterpret_cast<const char*>(&cosRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&sinRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&tanRet), sizeof(float));

		printf("cos comparison : (%.10f) - (%.10f)\n", cosRet, std::cos(trigInput));
		printf("sin comparison : (%.10f) - (%.10f)\n", sinRet, std::sin(trigInput));
		printf("tan comparison : (%.10f) - (%.10f)\n", tanRet, std::tan(trigInput));

		LAB_constexpr float arcInput = 0.5f;
		LAB_constexpr float arcCosRet = LAB::SupportingMath::ArcCos(arcInput);
		LAB_constexpr float arcSinRet = LAB::SupportingMath::ArcSin(arcInput);
		LAB_constexpr float arcTanRet = LAB::SupportingMath::ArcTan(arcInput);
		LAB_constexpr float arcTan2Ret = LAB::SupportingMath::ArcTan2(arcInput, 1.f);
		outFile.write(reinterpret_cast<const char*>(&arcCosRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&arcSinRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&arcTanRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&arcTan2Ret), sizeof(float));
		printf("\narccos comparison : (%.10f) - (%.10f)\n", arcCosRet, std::acos(arcInput));
		printf("\narcsin comparison : (%.10f) - (%.10f)\n", arcSinRet, std::asin(arcInput));
		printf("\narctan comparison : (%.10f) - (%.10f) - (%.10f)\n", arcTanRet, std::atan(arcInput), arcTan2Ret);

		//printf("arc tan comparison : (%.10f):(%.10f)\n", LAB::SupportingMath::ArcTan2BitMasking(trigInput, 1.f), LAB::SupportingMath::ArcTan2(trigInput, 1.f));
	}
	{ //other math functions
		LAB_constexpr float truncRet = LAB::SupportingMath::Trunc(11.f);
		LAB_constexpr float truncRet2 = LAB::SupportingMath::Trunc(11.2f);
		LAB_constexpr float truncRet3 = LAB::SupportingMath::Trunc(0.1f);
		LAB_constexpr float truncRet4 = LAB::SupportingMath::Trunc(-1.1f);
		outFile.write(reinterpret_cast<const char*>(&truncRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&truncRet2), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&truncRet3), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&truncRet4), sizeof(float));

		LAB_constexpr float modRet = LAB::SupportingMath::Mod(22.f, LAB::SupportingMath::GetPI<float, true>(2.f));
		outFile.write(reinterpret_cast<const char*>(&modRet), sizeof(float));
		printf("mod comparison : (%.10f):(%.10f)\n", modRet, std::fmod(22.f, LAB::SupportingMath::GetPI<float, true>(2.f)));
		printf("mod comparison : (%.10f):(%.10f)\n", LAB::SupportingMath::Mod(-25.f, LAB::SupportingMath::GetPI<float, true>(2.f)), std::fmod(-25.f, LAB::SupportingMath::GetPI<float, true>(2.f)));

		LAB_constexpr float piPhase1 = LAB::SupportingMath::PhaseToPi(LAB::SupportingMath::GetPI(2.f), -LAB::SupportingMath::GetPI_DividedBy(2.f), LAB::SupportingMath::GetPI_DividedBy(2.f));
		LAB_constexpr float piPhase2 = LAB::SupportingMath::PhaseToPi(-LAB::SupportingMath::PI<float>, -LAB::SupportingMath::GetPI_DividedBy(2.f), LAB::SupportingMath::GetPI_DividedBy(2.f));
		LAB_constexpr float piPhase3 = LAB::SupportingMath::PhaseToPi(4.f, -LAB::SupportingMath::GetPI_DividedBy(2.f), LAB::SupportingMath::GetPI_DividedBy(2.f));

		outFile.write(reinterpret_cast<const char*>(&piPhase1), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&piPhase2), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&piPhase3), sizeof(float));
		printf("pi phase - (%.10f):(%.10f):(%.10f)\n", piPhase1, piPhase2, piPhase3);
	}
	float testBreak = LAB::SupportingMath::InverseSqrt(1.f);
	printf("result : %.2f\n", testBreak);

	printf("made it to the end\n");
	return EXIT_SUCCESS;
}
#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"
#include "Camera.h"
#include "Vector/Hash.h"

#include <cstdio>
#include <fstream>
#include <concepts>
#include <type_traits>

#include <cmath>

using MyCS = lab::CoordinateSystem<lab::Direction::XDir<true>, lab::Direction::YDir<true>, lab::Direction::ZDir<true>>;


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

#if 0 //force a compiler mismatch, for testing the github workflow comparison (it works)
#ifdef _MSC_VER
	outFile << 0.0;
#elif defined(__clang__)
	outFile << 1.0;
#elif defined(__GNUC__)
	outFile << 2.0;
#endif
#endif
	{//hash test
		std::size_t seed = 0;
		seed ^= std::hash<lab::vec2>{}(lab::vec2(1.f, 0.f));
	}
	{ //oblong matrix
		lab::Matrix<float, 3, 2> mat32{std::array{lab::Vector<float, 2>(1.f), lab::Vector<float, 2>{2.f}, lab::Vector<float, 2>(3.f)}};
		lab::Matrix<float, 2, 3> mat23 = mat32.Transposed();

		outFile.write(reinterpret_cast<const char*>(&mat23), sizeof(mat23));
	}

	{ //vector constructor testing
		lab::ivec2 test{1};
		printf("int vec test : %d\n", test.x);

		lab::Vector<float, 3> testCon{lab::Vector<float, 2>{1.f}, 1.f};
		lab::Vector<float, 3> testCon2{1.f, lab::Vector<float, 2>{1.f}};
		printf("con test - %.2f:%.2f\n", testCon.x, testCon2.y);

		lab::Vector<float, 3> truncCon{lab::Vector<float, 4>{1.f}};
		printf("trunc con - %.2f\n", truncCon.x);
	}

	{ //matrix multiplcation, inverse transpose
		LAB_constexpr lab::Matrix<float, 4, 4> test1(0.f);
		LAB_constexpr lab::Matrix<float, 4, 4> test2(1.f);

		LAB_constexpr lab::Matrix<float, 4 ,4> test3 = test1 * test2;
		LAB_constexpr lab::Vector<float, 4> test4 = test1 * lab::Vector<float, 4>{1.f};
		printf("test mat multiplpication print : %.2f - %.2f\n", test3.columns[0][0], test4.x);

		lab::Matrix<float, 4, 4> crossTest{1.f};
		lab::Matrix<float, 4, 4> crossTest2{2.f};
		lab::Matrix<float, 4, 4> crossTestOut = crossTest * crossTest2;
		outFile.write(reinterpret_cast<const char*>(&crossTestOut.columns), sizeof(float) * 4 * 4);

		const auto normalMat = lab::Matrix<float, 3, 3>(5.f).GetInverse().Transposed();
		outFile.write(reinterpret_cast<const char*>(&normalMat), sizeof(normalMat));
	}

	{ //vectors
		LAB_constexpr lab::Vector<float, 3> vec3a(1.0f, 2.0f, 3.0f);
		LAB_constexpr lab::Vector<float, 3> vec3b(3.0f, 4.0f, 5.0f);
		LAB_constexpr lab::Vector<float, 3> vec3c(5.0f, 6.0f, 7.0f);
		LAB_constexpr lab::Vector<float, 3> vec3d(7.0f, 8.0f, 9.0f);
		lab::Cross((vec3a + vec3b) * vec3c - vec3d, vec3a);
		LAB_constexpr lab::Vector<float, 3> vec3aN = vec3a.Normalized();
		LAB_constexpr lab::Vector<float, 3> vec3bN = vec3b.Normalized();
		LAB_constexpr lab::Vector<float, 3> vec3cN = vec3c.Normalized();
		LAB_constexpr lab::Vector<float, 3> vec3dN = vec3d.Normalized();
		printf("normalized dots - %.2f\n", vec3aN.Dot(vec3bN) + vec3cN.Dot(vec3dN));

		LAB_constexpr lab::Matrix<float, 3, 2> imbalancedMat(0.f);
		printf("imbalanced mat usage : %.2f\n", imbalancedMat.At(2, 1));

		lab::Vector<float, 2> myVec{ 1.f, 2.f };
		myVec /= 2.f;
		LAB_constexpr float nDP = lab::NormalizedDot(vec3a, vec3b);

		outFile.write(reinterpret_cast<const char*>(&nDP), sizeof(float));
		//printf("nDP : %.2f\n", nDP);
	}

	{ //matrices

		LAB_constexpr lab::Vector<float, 2> checkVec1(1.f, 2.f);
		LAB_constexpr lab::Vector<float, 2> checkVec2(1.f / 2.f, 2.f / 2.f);
		LAB_static_assert((checkVec1 / 2.f) == checkVec2);

		LAB_constexpr lab::Vector<float, 2> checkVec3(3.f, 4.f);

		LAB_constexpr lab::Matrix<float, 2, 2> checkMat{{checkVec1, checkVec3 }};
		LAB_static_assert(checkMat.At(1, 1) == checkVec3.y);
		printf("chekc mat print : %.2f\n", checkMat.At(1, 1));
		//LAB_constexpr std::array<lab::Vector<float, 3>, 3> vecArray{ lab::Vector<float, 3>(0.f, 1.f, 2.f), lab::Vector<float, 3>(2.f, 3.f, 4.f), lab::Vector<float, 3>(3.f, 4.f, 5.f) };
		//lab::Matrix<float, 3, 3> checkMatNonConst(0.f);
		//checkMatNonConst.columns[0] = vecArray[0];

		//LAB_constexpr lab::Matrix<float, 3, 3, 4> mat1(vecArray);
		/*
		LAB_constexpr lab::Matrix<float, 3, 3> mat2{ lab::Vector<float, 3>(10.f, 11.f, 12.f), lab::Vector<float, 3>(22.f, 23.f, 24.f), lab::Vector<float, 3>(33.f, 34.f, 35.f) };

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
		LAB_constexpr lab::Vector<float, 3> testVec{ 0.f, 1.f, 2.f };
		LAB_constexpr float testFloat0 = testVec[0];
		LAB_constexpr float testFloat1 = testVec[1];
		LAB_static_assert(testFloat0 != testFloat1);

		LAB_constexpr lab::Matrix<float, 4, 4> matrix1(1.f);
		LAB_constexpr auto rotatedMat = lab::Rotate(matrix1, 0.5f, MyCS::unitUpVector);

		LAB_constexpr auto translatedMat = lab::Translate(rotatedMat, lab::Vector<float, 3>{0.f, 1.f, 2.f});
		LAB_constexpr auto scaledMat = lab::Scale(translatedMat, testVec);
		LAB_static_assert(scaledMat.At(0, 0) != scaledMat.At(3, 2));
		printf("scaledMat 0 - %.2f\n", scaledMat.At(0, 0));

		LAB_constexpr lab::Vector<float, 4> preMultVec{ 1.f };
		LAB_constexpr lab::Vector<float, 4> postMultVec = scaledMat * preMultVec;
		printf("postMultVec usage : %.2f\n", postMultVec.x);
	}
	{
	}
	{ //rotation matrices, scale matrix
		LAB_constexpr lab::Transform<float, 3> transformX{lab::Vector<float, 3>{0.f}, lab::Vector<float, 3>{1.f}, lab::Vector<float, 3>{lab::PI<float>, 0.f, 0.f}};
		LAB_constexpr lab::Transform<float, 3> transformY{lab::Vector<float, 3>{0.f}, lab::Vector<float, 3>{1.f}, lab::Vector<float, 3>{0.f, lab::PI<float>, 0.f}};
		LAB_constexpr lab::Transform<float, 3> transformZ{lab::Vector<float, 3>{0.f}, lab::Vector<float, 3>{1.f}, lab::Vector<float, 3>{0.f, 0.f, lab::PI<float>}};

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
		
		LAB_constexpr lab::Transform<float, 3> transformScale{};
		LAB_constexpr auto scaleMat = transformScale.GetScaleMatrix();
		LAB_constexpr lab::Matrix<float, 4, 4> identityMat{1.f};
		LAB_static_assert(scaleMat.columns[0][0] == 1.f);
		LAB_static_assert(identityMat.columns[0][0] == 1.f);

		LAB_static_assert(scaleMat == identityMat);
		printf("scale and identity - %.2f:%.2f\n", scaleMat.columns[0][0], identityMat.columns[0][0]);

		auto normMat = transformScale.GetNormalMatrix();
		printf("normMat 0,0 - %.2f\n", normMat.At(0, 0));
	}
	{ //camera functions
		LAB_constexpr auto proj = lab::ProjectionMatrix<lab::Perspective::Vulkan>(lab::DegreesToRadians(70.f), 1.44f, 0.f, 100.f);
	
		for(uint8_t x = 0; x < 4; x++){
			for(uint8_t y = 0; y < 4; y++){
				const float tempVal = proj.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&tempVal), sizeof(float));
			}
		}
		LAB_constexpr auto viewMat = lab::ViewDirection<MyCS>(lab::Vector<float, 3>(0.f), MyCS::unitForwardVector);
		for (uint8_t x = 0; x < 4; x++) {
			for (uint8_t y = 0; y < 4; y++) {
				const float tempVal = viewMat.At(x, y);
				outFile.write(reinterpret_cast<const char*>(&tempVal), sizeof(float));
			}
		}
		lab::mat4 tempMat(0.f);
		lab::ViewRotation<MyCS>(tempMat, lab::vec3(0.f), MyCS::unitForwardVector);
		outFile.write(reinterpret_cast<const char*>(&tempMat), sizeof(tempMat));
	}
	{ //trig functions
		LAB_constexpr float trigInput = 50.f;
		LAB_constexpr auto cosRet = lab::Cos(trigInput);
		LAB_constexpr auto sinRet = lab::Sin(trigInput);
		LAB_constexpr auto tanRet = lab::Tan(trigInput);

		outFile.write(reinterpret_cast<const char*>(&cosRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&sinRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&tanRet), sizeof(float));

		printf("cos comparison : (%.10f) - (%.10f)\n", cosRet, std::cos(trigInput));
		printf("sin comparison : (%.10f) - (%.10f)\n", sinRet, std::sin(trigInput));
		printf("tan comparison : (%.10f) - (%.10f)\n", tanRet, std::tan(trigInput));

		LAB_constexpr float arcInput = 0.5f;
		LAB_constexpr float arcCosRet = lab::ArcCos(arcInput);
		LAB_constexpr float arcSinRet = lab::ArcSin(arcInput);
		LAB_constexpr float arcTanRet = lab::ArcTan(arcInput);
		LAB_constexpr float arcTan2Ret = lab::ArcTan2(arcInput, 1.f);
		outFile.write(reinterpret_cast<const char*>(&arcCosRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&arcSinRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&arcTanRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&arcTan2Ret), sizeof(float));
		printf("\narccos comparison : (%.10f) - (%.10f)\n", arcCosRet, std::acos(arcInput));
		printf("\narcsin comparison : (%.10f) - (%.10f)\n", arcSinRet, std::asin(arcInput));
		printf("\narctan comparison : (%.10f) - (%.10f) - (%.10f)\n", arcTanRet, std::atan(arcInput), arcTan2Ret);

		//printf("arc tan comparison : (%.10f):(%.10f)\n", lab::ArcTan2BitMasking(trigInput, 1.f), lab::ArcTan2(trigInput, 1.f));
	}
	{ //other math functions
		LAB_constexpr float truncRet = lab::Trunc(11.f);
		LAB_constexpr float truncRet2 = lab::Trunc(11.2f);
		LAB_constexpr float truncRet3 = lab::Trunc(0.1f);
		LAB_constexpr float truncRet4 = lab::Trunc(-1.1f);
		outFile.write(reinterpret_cast<const char*>(&truncRet), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&truncRet2), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&truncRet3), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&truncRet4), sizeof(float));

		LAB_constexpr float modRet = lab::Mod(22.f, lab::GetPI<float, true>(2.f));
		outFile.write(reinterpret_cast<const char*>(&modRet), sizeof(float));
		printf("mod comparison : (%.10f):(%.10f)\n", modRet, std::fmod(22.f, lab::GetPI<float, true>(2.f)));
		printf("mod comparison : (%.10f):(%.10f)\n", lab::Mod(-25.f, lab::GetPI<float, true>(2.f)), std::fmod(-25.f, lab::GetPI<float, true>(2.f)));

		LAB_constexpr float piPhase1 = lab::PhaseTo(lab::GetPI(2.f), -lab::GetPI_DividedBy(2.f), lab::GetPI_DividedBy(2.f));
		LAB_constexpr float piPhase2 = lab::PhaseTo(-lab::PI<float>, -lab::GetPI_DividedBy(2.f), lab::GetPI_DividedBy(2.f));
		LAB_constexpr float piPhase3 = lab::PhaseTo(4.f, -lab::GetPI_DividedBy(2.f), lab::GetPI_DividedBy(2.f));

		outFile.write(reinterpret_cast<const char*>(&piPhase1), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&piPhase2), sizeof(float));
		outFile.write(reinterpret_cast<const char*>(&piPhase3), sizeof(float));
		printf("pi phase - (%.10f):(%.10f):(%.10f)\n", piPhase1, piPhase2, piPhase3);
	}
	float testBreak = lab::InverseSqrt(1.f);
	printf("result : %.2f\n", testBreak);

	printf("made it to the end\n");
	return EXIT_SUCCESS;
}
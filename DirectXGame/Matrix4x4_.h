// Matrix4x4_.h
#pragma once
#include "KamataEngine.h"
#include "Matrix4x4_.h"
#include <cmath>
namespace KamataEngine {
class Matrix4x4_ {
public:
	float m[4][4];

	Matrix4x4_();

	static Matrix4x4_ MakeIdentity4x4();
	static Matrix4x4_ Translation(const Vector3& translation);
	static Matrix4x4_ Scale(const Vector3& scale);
	static Matrix4x4_ RotateY(float angleRad);
	static Matrix4x4_ RotateXYZ(float angleX, float angleY, float angleZ);
	static Matrix4x4_ PerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	static Matrix4x4_ Multiply(const Matrix4x4_& m1, const Matrix4x4_& m2);
	static Matrix4x4_ MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translation);
	static Matrix4x4_ Inverse(const Matrix4x4_& m);
};
} // namespace KamataEngine
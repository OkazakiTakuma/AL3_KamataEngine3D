#include "Matrix.h"
#include <assert.h>
#include <cmath>
#include <math.h>
static const int kRowHeight = 20;
static const int kColumnWidth = 60;
static const float p = 3.1415f;
using namespace KamataEngine;
KamataEngine::Matrix4x4 Add(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2) {
	KamataEngine::Matrix4x4 add{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			add.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return add;
};
KamataEngine::Matrix4x4 Subtract(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2) {
	KamataEngine::Matrix4x4 subtract{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			subtract.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return subtract;
};
KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2) {
	KamataEngine::Matrix4x4 multiply{};
	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {
			multiply.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}

	return multiply;
};
KamataEngine::Matrix4x4 Inverse(const KamataEngine::Matrix4x4& m) {
	float det =
	    m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	    m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	    m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	    m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	    m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	    m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	KamataEngine::Matrix4x4 Inverse{};
	Inverse.m[0][0] = 1 / det *
	                  (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
	                   m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	Inverse.m[0][1] = 1 / det *
	                  (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
	                   m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	Inverse.m[0][2] = 1 / det *
	                  (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
	                   m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	Inverse.m[0][3] = 1 / det *
	                  (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
	                   m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	Inverse.m[1][0] = 1 / det *
	                  (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
	                   m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	Inverse.m[1][1] = 1 / det *
	                  (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
	                   m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	Inverse.m[1][2] = 1 / det *
	                  (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
	                   m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	Inverse.m[1][3] = 1 / det *
	                  (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
	                   m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

	Inverse.m[2][0] = 1 / det *
	                  (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
	                   m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	Inverse.m[2][1] = 1 / det *
	                  (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
	                   m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	Inverse.m[2][2] = 1 / det *
	                  (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
	                   m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	Inverse.m[2][3] = 1 / det *
	                  (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
	                   m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

	Inverse.m[3][0] = 1 / det *
	                  (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
	                   m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	Inverse.m[3][1] = 1 / det *
	                  (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
	                   m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	Inverse.m[3][2] = 1 / det *
	                  (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
	                   m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	Inverse.m[3][3] = 1 / det *
	                  (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
	                   m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
	return Inverse;
};
KamataEngine::Matrix4x4 Transpose(const KamataEngine::Matrix4x4& m) {
	KamataEngine::Matrix4x4 trancepose{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			trancepose.m[i][j] = m.m[j][i];
		}
	}

	return trancepose;
};
KamataEngine::Matrix4x4 MakeIdentiy4x4() {
	KamataEngine::Matrix4x4 identiy = {0};
	for (int i = 0; i < 4; i++) {
		identiy.m[i][i] = 1.0f;
	}

	return identiy;
}

KamataEngine::Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	KamataEngine::Matrix4x4 translateMatrix = MakeIdentiy4x4();
	translateMatrix.m[3][0] = translate.x;
	translateMatrix.m[3][1] = translate.y;
	translateMatrix.m[3][2] = translate.z;

	return translateMatrix;
}

KamataEngine::Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	KamataEngine::Matrix4x4 scaleMatrix = MakeIdentiy4x4();
	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[2][2] = scale.z;
	return scaleMatrix;
}

KamataEngine::Matrix4x4 MakeRotateXMatrix(float radiun) {
	KamataEngine::Matrix4x4 rotateX = MakeIdentiy4x4();
	rotateX.m[1][1] = std::cos(radiun);
	rotateX.m[2][1] = -std::sin(radiun);
	rotateX.m[1][2] = std::sin(radiun);
	rotateX.m[2][2] = std::cos(radiun);

	return rotateX;
}

KamataEngine::Matrix4x4 MakeRotateYMatrix(float radiun) {
	KamataEngine::Matrix4x4 rotateY = MakeIdentiy4x4();
	rotateY.m[0][0] = std::cos(radiun);
	rotateY.m[2][0] = std::sin(radiun);
	rotateY.m[0][2] = -std::sin(radiun);
	rotateY.m[2][2] = std::cos(radiun);

	return rotateY;
}

KamataEngine::Matrix4x4 MakeRotateZMatrix(float radiun) {
	KamataEngine::Matrix4x4 rotateZ = MakeIdentiy4x4();
	rotateZ.m[0][0] = std::cos(radiun);
	rotateZ.m[1][0] = -std::sin(radiun);
	rotateZ.m[0][1] = std::sin(radiun);
	rotateZ.m[1][1] = std::cos(radiun);

	return rotateZ;
}

KamataEngine::Matrix4x4 MakeRotateXYZMatrix(Vector3 rotate) {
	KamataEngine::Matrix4x4 XMatrix = MakeRotateXMatrix(rotate.x);
	KamataEngine::Matrix4x4 YMatrix = MakeRotateYMatrix(rotate.y);
	KamataEngine::Matrix4x4 ZMatrix = MakeRotateZMatrix(rotate.z);
	KamataEngine::Matrix4x4 XYZMatrix = Multiply(XMatrix, Multiply(YMatrix, ZMatrix));

	return XYZMatrix;
}

KamataEngine::Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	KamataEngine::Matrix4x4 affineMatrix;
	KamataEngine::Matrix4x4 rotateMatirix = MakeRotateXYZMatrix(rotate);
	affineMatrix.m[0][0] = scale.x * rotateMatirix.m[0][0];
	affineMatrix.m[1][0] = scale.y * rotateMatirix.m[1][0];
	affineMatrix.m[2][0] = scale.z * rotateMatirix.m[2][0];
	affineMatrix.m[3][0] = translate.x;

	affineMatrix.m[0][1] = scale.x * rotateMatirix.m[0][1];
	affineMatrix.m[1][1] = scale.y * rotateMatirix.m[1][1];
	affineMatrix.m[2][1] = scale.z * rotateMatirix.m[2][1];
	affineMatrix.m[3][1] = translate.y;

	affineMatrix.m[0][2] = scale.x * rotateMatirix.m[0][2];
	affineMatrix.m[1][2] = scale.y * rotateMatirix.m[1][2];
	affineMatrix.m[2][2] = scale.z * rotateMatirix.m[2][2];
	affineMatrix.m[3][2] = translate.z;

	affineMatrix.m[0][3] = 0.0f;
	affineMatrix.m[1][3] = 0.0f;
	affineMatrix.m[2][3] = scale.z * rotateMatirix.m[2][3];
	affineMatrix.m[3][3] = 1.0f;
	return affineMatrix;
}

Vector3 Transform(const Vector3& vector, const KamataEngine::Matrix4x4& matrix) {
	Vector3 result{};

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

bool IsCollisionAABBToAABB(const AABB& a, const AABB& b) {
	if (a.min.x <= b.max.x && a.max.x >= b.min.x && a.min.y <= b.max.y && a.max.y >= b.min.y && a.min.z <= b.max.z && a.max.z >= b.min.z) {
		return true;
	} else {

		return false;
	}
}
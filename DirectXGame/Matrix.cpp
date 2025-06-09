#include "Matrix.h"
#include <assert.h>
#include <cmath>
#include <math.h>
static const int kRowHeight = 20;
static const int kColumnWidth = 60;
static const float p = 3.1415f;
Matrix4x4Afifne Add(const Matrix4x4Afifne& m1, const Matrix4x4Afifne& m2) {
	Matrix4x4Afifne add{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			add.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return add;
};
Matrix4x4Afifne Subtract(const Matrix4x4Afifne& m1, const Matrix4x4Afifne& m2) {
	Matrix4x4Afifne subtract{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			subtract.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return subtract;
};
Matrix4x4Afifne Multiply(const Matrix4x4Afifne& m1, const Matrix4x4Afifne& m2) {
	Matrix4x4Afifne multiply{};
	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {
			multiply.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}

	return multiply;
};
Matrix4x4Afifne Inverse(const Matrix4x4Afifne& m) {
	float det =
	    m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	    m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	    m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	    m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	    m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	    m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4Afifne Inverse{};
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
Matrix4x4Afifne Transpose(const Matrix4x4Afifne& m) {
	Matrix4x4Afifne trancepose{};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			trancepose.m[i][j] = m.m[j][i];
		}
	}

	return trancepose;
};
Matrix4x4Afifne MakeIdentiy4x4() {
	Matrix4x4Afifne identiy = {0};
	for (int i = 0; i < 4; i++) {
		identiy.m[i][i] = 1.0f;
	}

	return identiy;
}

Matrix4x4Afifne MakeTranslateMatrix(const Vector3Matrix& translate) {
	Matrix4x4Afifne translateMatrix = MakeIdentiy4x4();
	translateMatrix.m[3][0] = translate.x;
	translateMatrix.m[3][1] = translate.y;
	translateMatrix.m[3][2] = translate.z;

	return translateMatrix;
}

Matrix4x4Afifne MakeScaleMatrix(const Vector3Matrix& scale) {
	Matrix4x4Afifne scaleMatrix = MakeIdentiy4x4();
	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[2][2] = scale.z;
	return scaleMatrix;
}

Matrix4x4Afifne MakeRotateXMatrix(float radiun) {
	Matrix4x4Afifne rotateX = MakeIdentiy4x4();
	rotateX.m[1][1] = std::cos(radiun);
	rotateX.m[2][1] = -std::sin(radiun);
	rotateX.m[1][2] = std::sin(radiun);
	rotateX.m[2][2] = std::cos(radiun);

	return rotateX;
}

Matrix4x4Afifne MakeRotateYMatrix(float radiun) {
	Matrix4x4Afifne rotateY = MakeIdentiy4x4();
	rotateY.m[0][0] = std::cos(radiun);
	rotateY.m[2][0] = std::sin(radiun);
	rotateY.m[0][2] = -std::sin(radiun);
	rotateY.m[2][2] = std::cos(radiun);

	return rotateY;
}

Matrix4x4Afifne MakeRotateZMatrix(float radiun) {
	Matrix4x4Afifne rotateZ = MakeIdentiy4x4();
	rotateZ.m[0][0] = std::cos(radiun);
	rotateZ.m[1][0] = -std::sin(radiun);
	rotateZ.m[0][1] = std::sin(radiun);
	rotateZ.m[1][1] = std::cos(radiun);

	return rotateZ;
}

Matrix4x4Afifne MakeRotateXYZMatrix(Vector3Matrix rotate) {
	Matrix4x4Afifne XMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4Afifne YMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4Afifne ZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4Afifne XYZMatrix = Multiply(XMatrix, Multiply(YMatrix, ZMatrix));

	return XYZMatrix;
}

Matrix4x4Afifne MakeAffineMatrix(Vector3Matrix scale, Vector3Matrix rotate, Vector3Matrix translate) {
	Matrix4x4Afifne affineMatrix;
	Matrix4x4Afifne rotateMatirix = MakeRotateXYZMatrix(rotate);
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

Vector3Matrix Transform(const Vector3Matrix& vector, const Matrix4x4Afifne& matrix) {
	Vector3Matrix result{};

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

#pragma once
#include "Vector.h"
/// <summary>
/// 4x4の行列
/// </summary>
struct Matrix4x4Afifne {
	float m[4][4];
};

// 行列の加法
Matrix4x4Afifne Add(const Matrix4x4Afifne& m1, const Matrix4x4Afifne& m2);
// 行列の減法
Matrix4x4Afifne Subtract(const Matrix4x4Afifne& m1, const Matrix4x4Afifne& m2);
// 行列の積
Matrix4x4Afifne Multiply(const Matrix4x4Afifne& m1, const Matrix4x4Afifne& m2);
// 逆行列
Matrix4x4Afifne Inverse(const Matrix4x4Afifne& m);
// 転置行列
Matrix4x4Afifne Transpose(const Matrix4x4Afifne& m);
// 単位行列の作成
Matrix4x4Afifne MakeIdentiy4x4();
// 平行移動行列
Matrix4x4Afifne MakeTranslateMatrix(const Vector3Matrix& translate);
// 拡大縮小行列
Matrix4x4Afifne MakeScaleMatrix(const Vector3Matrix& scale);
// X軸回転行列
Matrix4x4Afifne MakeRotateXMatrix(float radiun);
// Y軸回転行列
Matrix4x4Afifne MakeRotateYMatrix(float radiun);
// Z軸回転行列
Matrix4x4Afifne MakeRotateZMatrix(float radiun);
// XYZ回転行列
Matrix4x4Afifne MakeRotateXYZMatrix(Vector3Matrix radiun);
// アフィン変換
Matrix4x4Afifne MakeAffineMatrix(Vector3Matrix scale, Vector3Matrix rotate, Vector3Matrix translate);
// 座標変換
Vector3Matrix Transform(const Vector3Matrix& vector, const Matrix4x4Afifne& matrix);

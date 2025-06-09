#pragma once
#include<cmath>

struct Vector3Matrix {
	float x, y, z;
};
// 加算
Vector3Matrix Add(const Vector3Matrix& v1, const Vector3Matrix& v2);
// 減算
Vector3Matrix Subtract(const Vector3Matrix& v1, const Vector3Matrix& v2);
// スカラー倍
Vector3Matrix Multiply(float scalar, const Vector3Matrix& v);
// 内積
float Dot(const Vector3Matrix& v1, const Vector3Matrix& v2);
// 長さ (ノルム)
float Length(const Vector3Matrix& v);
// 正規化
Vector3Matrix Normalize(const Vector3Matrix& v);

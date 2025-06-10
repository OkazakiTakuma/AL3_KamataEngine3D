#pragma once
#include "Vector.h"
#include"KamataEngine.h"
struct Sphere {
	KamataEngine::Vector3 center; // 球の中心
	float radius;   // 球の半径
};
struct Line {
	KamataEngine::Vector3 origin; // 線の始点
	KamataEngine::Vector3 diff; // 線の終点
};
struct Ray {
	KamataEngine::Vector3 origin; // 線の始点
	KamataEngine::Vector3 diff; // 線の終点
};
struct Segment {
	KamataEngine::Vector3 origin; // 線の始点
	KamataEngine::Vector3 diff; // 線の終点
};
struct Plane {
	KamataEngine::Vector3 normal; //!< 法線
	float distance; //!< 距離
};
struct Triangle {
	KamataEngine::Vector3 vertices[3]; //!< 頂点
};
struct AABB {
	KamataEngine::Vector3 min; //!< 最小点
	KamataEngine::Vector3 max; //!< 最大点
};
struct OBB {
	KamataEngine::Vector3 center;         //!< 中心
	KamataEngine::Vector3 orientation[3]; //!< 軸
	KamataEngine::Vector3 size;           //!< 半径
};
// 行列の加法
KamataEngine::Matrix4x4 Add(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
// 行列の減法
KamataEngine::Matrix4x4 Subtract(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
// 行列の積
KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& m1, const KamataEngine::Matrix4x4& m2);
// 逆行列
KamataEngine::Matrix4x4 Inverse(const KamataEngine::Matrix4x4& m);
// 転置行列
KamataEngine::Matrix4x4 Transpose(const KamataEngine::Matrix4x4& m);
// 単位行列の作成
KamataEngine::Matrix4x4 MakeIdentiy4x4();
// 平行移動行列
KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translate);
// 拡大縮小行列
KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale);
// X軸回転行列
KamataEngine::Matrix4x4 MakeRotateXMatrix(float radiun);
// Y軸回転行列
KamataEngine::Matrix4x4 MakeRotateYMatrix(float radiun);
// Z軸回転行列
KamataEngine::Matrix4x4 MakeRotateZMatrix(float radiun);
// XYZ回転行列
KamataEngine::Matrix4x4 MakeRotateXYZMatrix(KamataEngine::Vector3 radiun);
// アフィン変換
KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3 scale, KamataEngine::Vector3 rotate, KamataEngine::Vector3 translate);
// 座標変換
KamataEngine::Vector3 Transform(const KamataEngine::Vector3& vector, const KamataEngine::Matrix4x4& matrix);
bool IsCollisionAABBToAABB(const AABB& aabb1, const AABB& aabb2);

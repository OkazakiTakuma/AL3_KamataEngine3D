#include "WorldTransformClass.h"
using namespace KamataEngine;
using namespace MathUtility;

//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);


Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	return scaleMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * translateMatrix;
}

void WorldTrnasformUpdate(KamataEngine::WorldTransform& worldTransform) {

	// スケール、回転、平行移動を合成して行列を計算する
	worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	// 定数バッファへの書き込み
	worldTransform.TransferMatrix();
}


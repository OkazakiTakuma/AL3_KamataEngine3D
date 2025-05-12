#include "worldMatrix.h"

using namespace KamataEngine;

void WorldTransformUpdate(KamataEngine::WorldTransform& worldTransform){
      // scaleの変換
	Vector3Matrix scale = {worldTransform.scale_.x, worldTransform.scale_.y, worldTransform.scale_.z};
	
	// rotateの変換
	Vector3Matrix rotate = {worldTransform.rotation_.x, worldTransform.rotation_.y, worldTransform.rotation_.z};

	// translateの変換
	Vector3Matrix translate = {worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z};

// アフィン変換
Matrix4x4Afifne affine = MakeAffineMatrix(scale, rotate, translate);
for (int i = 0; i < 4; i++) {
	for (int j = 0; j < 4; j++) {
		// ワールドトランスフォームの行列にアフィン変換を適用
		worldTransform.matWorld_.m[j][i] = affine.m[j][i];
	}
}
// ワールドトランスフォームの転送
worldTransform.TransferMatrix();
}

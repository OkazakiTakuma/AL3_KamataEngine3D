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

Vector3 Leap(Vector3 startPosition, Vector3 endPosition, float addTimer) {
	// 時間を 0.0 ～ 1.0 の範囲に制限
	addTimer = std::clamp(addTimer, 0.0f, 1.0f);

	// 線形補間 (Lerp)
	Vector3 result;
	result.x = startPosition.x + (endPosition.x - startPosition.x) * addTimer;
	result.y = startPosition.y + (endPosition.y - startPosition.y) * addTimer;
	result.z = startPosition.z + (endPosition.z - startPosition.z) * addTimer;

	return result;


}

#include "DeathParticles.h"

using namespace KamataEngine;
void DeathParticles::Initialize(const KamataEngine::Vector3& position, KamataEngine::Model* model) {
	model_ = model;
	// ワールドトランスフォームの初期化
	for (auto& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	textureHandle_ = 0; // テクスチャハンドルの初期化

	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f}; // パーティクルの色を白に設定
}

void DeathParticles::Update() {
	// パーティクルの更新処理

	if (isFInished_) {
		return; // パーティクルが終了している場合は更新しない
	}

	counter_ += 1.0f / 60.0f; // カウンターを更新
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f); // アルファ値を減少させる
	objectColor_.SetColor(color_);
	if (counter_ >= kDuration) {
		isFInished_ = true; // パーティクルの終了フラグを立てる
	}
	for (uint32_t i = 0; i < kNumParticles; ++i) {
		
		Vector3 velocity = {kSpeed, 0, 0}; // パーティクルの速度
		// パーティクルの角度を計算
		float angle = kAngkeunit * i;
		Matrix4x4 rotationMatrix = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, rotationMatrix);
		// パーティクルの位置を更新
		worldTransform_[i].translation_.x += velocity.x;
		worldTransform_[i].translation_.y += velocity.y;
		worldTransform_[i].translation_.z += velocity.z;
		
	}
	for (auto& worldTransform : worldTransform_) {
		WorldTransformUpdate(worldTransform);
	}


}

void DeathParticles::Draw(const KamataEngine::Camera* camera) {
	// パーティクルの描画処理
	if (isFInished_) {
		return; // パーティクルが終了している場合は描画しない
	}
	for (const auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera, textureHandle_, &objectColor_);
	}
}

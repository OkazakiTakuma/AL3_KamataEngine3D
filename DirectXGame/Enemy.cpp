#include "Enemy.h"
#include <numbers>
#include "worldMatrix.h"


void Enemy::Initialize(const KamataEngine::Vector3& position, KamataEngine::Model* model) {
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2; // 初期のY軸回転を設定

	velocity_ = {-kWalkSpeed, 0, 0}; // 初期速度を設定

	warkTimer_ = 0.0f; // ワークモーションタイマーの初期化

}

void Enemy::Update() {
	// 座標をもとに行列の更新をする
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;

	// ワークモーションの更新
	warkTimer_ += 1.0f / 60.0f; // タイマーを更新
	
	float param = std::sin(warkTimer_ * (std::numbers::pi_v<float> / kWarlMotionTime));
	float degree = kWorkMotionAngleStart + kWorkMotionAngleEnd * (param + 1.0f) / 2.0f; // ワークモーションの角度を計算
	worldTransform_.rotation_.x = std::sin(degree);                                               // Y軸の回転を設定
	
	WorldTransformUpdate(worldTransform_);

}

void Enemy::Draw(const KamataEngine::Camera* camera) { model_->Draw(worldTransform_, *camera, 0, nullptr);

}

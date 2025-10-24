#include "Enemy.h"
#include "Player.h"
#include "worldMatrix.h"
#include <cmath>
#include <numbers>

using namespace KamataEngine;


void Enemy::Initialize(const KamataEngine::Vector3& position, KamataEngine::Model* model) {
	model_ = model;
	isDead_ = false;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;

	velocity_.x = -kWalkSpeed;
	velocity_.y = 0.0f;
	velocity_.z = 0.0f;

	warkTimer_ = 0.0f;
}

void Enemy::Update() {
	if (isDead_)
		return;

	// 位置更新
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;

	// ワークモーションの更新（固定フレームデルタを使用）
	constexpr float kFrameDelta = 1.0f / 60.0f;
	warkTimer_ += kFrameDelta;

	// warkTimer が大きくなりすぎないように丸める
	if (warkTimer_ > 1e6f)
		warkTimer_ = std::fmod(warkTimer_, 2.0f * std::numbers::pi_v<float>);

	// ワークモーション角度の計算（元実装の式を保持）
	const float param = std::sin(warkTimer_ * (std::numbers::pi_v<float> / kWarlMotionTime));
	const float degree = kWorkMotionAngleStart + kWorkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = std::sin(degree);

	// ワールド行列の更新（外部関数を呼ぶ）
	WorldTransformUpdate(worldTransform_);
}

void Enemy::Draw(const KamataEngine::Camera* camera) {
	if (!model_)
		return;
	if (!camera)
		return;
	model_->Draw(worldTransform_, *camera);
}

KamataEngine::Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPosition;
	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];
	return worldPosition;
}

AABB Enemy::GetAABB() {
	AABB aabb;
	Vector3 worldPosition = GetWorldPosition();
	constexpr float half = 0.4f;
	aabb.min = worldPosition - Vector3(half, half, half);
	aabb.max = worldPosition + Vector3(half, half, half);
	return aabb;
}

void Enemy::OnCollisionPlayer(Player* /*player*/) { isDead_ = true; }

bool Enemy::GetIsDead() { return isDead_; }
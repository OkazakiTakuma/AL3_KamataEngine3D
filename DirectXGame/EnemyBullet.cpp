#include "EnemyBullet.h"
#include "Player.h"
using namespace KamataEngine;

#include <cmath> // std::atan2 用

Model* EnemyBullet::model_ = nullptr;

void EnemyBullet::LoadModel() { model_ = Model::CreateFromOBJ("enemy"); }

void EnemyBullet::Initialize( const Vector3& position, const Vector3& velocity, Player* player) {
	textureHandle_ = TextureManager::Load("black.png");
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	player_ = player; // ← ここが超重要！

	worldTransform_.scale_ = Vector3(0.5f, 0.5f, 3.0f);

	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	float horizontalLen = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, horizontalLen);

	WorldTrnasformUpdate(worldTransform_);

	SetCollisionAttribute(kCollisionAttributeEnemyBullet);
	SetCollisionMask(kCollisionAttributePlayer | kCollisionAttributePlayerBullet);


}

void EnemyBullet::OnCollision() {

	isDead_ = true; // 衝突したらデスフラグを立てる
}

KamataEngine::Vector3 EnemyBullet::GetPosition() {

	Vector3 worldPos;

	worldPos=worldTransform_.translation_;
	return worldPos;
}

void EnemyBullet::Update() {
	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//敵球から自キャラへのベクトルを計算
	Vector3 toPlayer = player_->GetPosition() - worldTransform_.translation_;
	//ベクトルを正規化
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);
	//球面政権補間
	velocity_ = Slerp(velocity_, toPlayer, 0.03f); // 進行方向をプレイヤーに向ける

	// 進行方向のY回転（XZ平面に投影して考える）
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// 進行方向のX回転（Y方向の傾き）
	float horizontalLen = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, horizontalLen); // ※右手系前提


	WorldTrnasformUpdate(worldTransform_);
}

void EnemyBullet::Draw(const Camera& camera) {

	model_->Draw(worldTransform_, camera, textureHandle_);

}
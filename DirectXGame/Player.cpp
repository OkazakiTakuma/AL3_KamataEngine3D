#include "Player.h"
using namespace KamataEngine;

void Player::Initialize(const Camera* camera) {

	model_ = Model::CreateFromOBJ("player", true);
	camera_ = camera;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();


}

Player::~Player() {
	delete model_;
	
	
}


void Player::Update() { // ワールドトランスフォームの更新
	// scaleの変換
	scale_.x = static_cast<float>(worldTransform_.scale_.x);
	scale_.y = static_cast<float>(worldTransform_.scale_.y);
	scale_.z = static_cast<float>(worldTransform_.scale_.z);
	// rotateの変換
	rotate_.x = static_cast<float>(worldTransform_.rotation_.x);
	rotate_.y = static_cast<float>(worldTransform_.rotation_.y);
	rotate_.z = static_cast<float>(worldTransform_.rotation_.z);
	// translateの変換
	translate_.x = static_cast<float>(worldTransform_.translation_.x);
	translate_.y = static_cast<float>(worldTransform_.translation_.y);
	translate_.z = static_cast<float>(worldTransform_.translation_.z);
	// アフィン変換
	Matrix4x4Afifne affine = MakeAffineMatrix(scale_, rotate_, translate_);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// ワールドトランスフォームの行列にアフィン変換を適用
			worldTransform_.matWorld_.m[j][i] = affine.m[j][i];
		}
	}
	// ワールドトランスフォームの転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);
	;
}
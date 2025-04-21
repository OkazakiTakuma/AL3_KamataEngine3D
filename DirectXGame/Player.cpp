#include "Player.h"
#include <assert.h>
using namespace KamataEngine;


void Player::Initialize(Model* model, uint32_t textureHandle, Camera* camera) {
	textstureHandle_ = textureHandle;
	assert(model);
	model_ = model;
	camera_ = camera;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

Player::~Player() {}

void Player::Update() { worldTransform_.TransferMatrix(); }

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_, textstureHandle_,nullptr);
}

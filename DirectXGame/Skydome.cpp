#include "Skydome.h"
using namespace KamataEngine;
using namespace MathUtility;

void Skydome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {

	// NULLポインタチェック
	assert(model);

	// 引数の内容をメンバ変数に記録
	model_ = model;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {100.0f, 100.0f, 100.0f}; // 大きめにしてみる
	worldTransform_.TransferMatrix();


}

void Skydome::Update() {}

void Skydome::Draw() {

	model_->Draw(worldTransform_, *camera_);

}
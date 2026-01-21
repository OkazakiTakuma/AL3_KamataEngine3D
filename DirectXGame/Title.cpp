#include "Title.h"

void Title::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	// モデルの読み込み
	model_ = model;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	basePosition_ = position; 
	worldTransform_.translation_ = basePosition_;
	// カメラの初期化
	camera_ = camera;
	// テクスチャハンドルの設定
	textureHandle_ = KamataEngine::TextureManager::Load("sample.png");
}

void Title::Update() {
	// タイトルシーンの更新処理
	

	// 上下に浮かぶようにY座標をアニメーション
	floatTimer_ += 0.05f; // 動きの速さ（大きくすると速くなる）

	// Y座標を元の位置から ±10 ピクセル範囲で上下させる
	worldTransform_.translation_.y = basePosition_.y + std::sinf(floatTimer_) * 10.0f;


	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	
	WorldTrnasformUpdate(worldTransform_);

}

void Title::Draw() {
	// 描画処理

	// タイトルモデルの描画
	if (model_) {
		model_->Draw(worldTransform_, *camera_, textureHandle_);
	}
}
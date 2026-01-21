#include "DethParticles.h"
#include <algorithm>
using namespace KamataEngine;
using namespace MathUtility;

void DethParticles::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	model_ = model;
	camera_ = camera;
	textureHandle_ = textureHandle;

	// 初期化処理
	for (KamataEngine::WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position; // 初期位置を設定
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1}; // 初期色を白に設定

}

void DethParticles::Updata() {
	
	if (isFinished_) {
		return; // すでに終了している場合は何もしない
	}

	//ワールド変換の更新
	for (KamataEngine::WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		worldTransform.TransferMatrix();
	}

	for (uint32_t i = 0; i < kNumParticleSize; ++i) {
		KamataEngine::Vector3 velosity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = i * kAngleUint;
		//z軸周りの回転行列
		KamataEngine::Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velosity = Transform(velosity, matrixRotation);
		//移動処理
		worldTransforms_[i].translation_.x += velosity.x;
		worldTransforms_[i].translation_.y += velosity.y;
		worldTransforms_[i].translation_.z += velosity.z;
	}

	//カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	// パーティクルの持続時間を超えたら終了フラグを立てる
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}
	color_.w = 1.0f - (counter_ / kDuration);
	color_.w = std::clamp(color_.w, 0.0f, 1.0f);
	objectColor_.SetColor(color_);

}

void DethParticles::Draw() {

	if (isFinished_) {
		return; // すでに終了している場合は何もしない
	}

	// 描画処理
	for (const KamataEngine::WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *camera_, textureHandle_,&objectColor_); // テクスチャハンドルは0を仮設定
	}


}
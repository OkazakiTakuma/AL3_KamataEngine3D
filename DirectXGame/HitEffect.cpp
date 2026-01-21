#include "HitEffect.h"

// 静的メンバ変数の実態
KamataEngine::Model* HitEffect::model_ = nullptr;
KamataEngine::Camera* HitEffect::camera_ = nullptr;

HitEffect* HitEffect::Create(KamataEngine::Vector3& spawnPosition) {
	HitEffect* instance = new HitEffect();
	// newの失敗を検出
	assert(instance);
	// インスタンスの初期化
	instance->Initialize(spawnPosition);
	// 初期化したインスタンスを返す
	return instance;
}

void HitEffect::Initialize(KamataEngine::Vector3& spawnPosition) {
	// ワールドトランスフォームの初期化
	circleWorldTransform.Initialize();
	// 初期位置を設定
	circleWorldTransform.translation_ = spawnPosition;
	// スケールを設定
	circleWorldTransform.scale_ = {1.0f, 1.0f, 1.0f};
	// 回転を設定
	circleWorldTransform.rotation_ = {0.0f, 0.0f, 0.0f};

	ellipseRotate = random_.GetFloat(-6.28318530718f / 2, 6.28318530718f / 2); // 0から2πの範囲でランダムな回転を設定

	for (KamataEngine::WorldTransform& worldTransform : ellipseworldTransforms_) {

		ellipseRotate = random_.GetFloat(-6.28318530718f / 2, 6.28318530718f / 2); // 0から2πの範囲でランダムな回転を設定

		worldTransform.scale_ = {0.1f, 1.5f, 1.0f}; // スケールを設定

		worldTransform.rotation_ = {
		    0.0f, 0.0f,
		    //-πからπの範囲でランダムな回転を設定
		    ellipseRotate}; // 初期位置を設定

		alpha_ = 1.0f; // 描画用アルファ値（1:不透明 → 0:透明）

		worldTransform.translation_ = {circleWorldTransform.translation_.x, circleWorldTransform.translation_.y, circleWorldTransform.translation_.z - 0.01f}; // 初期位置を設定
		worldTransform.Initialize();

		worldTransform.translation_ = circleWorldTransform.translation_;
	}

	color.Initialize();
	color.SetColor({1.0f, 1.0f, 1.0f, alpha_});
}

void HitEffect::Update() {
	// エフェクトの更新処理

	counter_ += 1.0f / 60.0f;

	switch (state_) {
	case State::Spread: {
		float t = counter_ / spreadDuration_;
		if (t >= 1.0f) {
			t = 1.0f;
			counter_ = 0.0f;
			state_ = State::Fade;
		}

		float scale = 1.0f + t * 0.3f; // 拡大（例: 1.0 → 2.0）
		circleWorldTransform.scale_ = {scale, scale, scale};
		for (auto& wt : ellipseworldTransforms_) {
			wt.scale_ = {0.1f * scale, 2.0f * scale, 1.0f * scale};
		}
		break;
	}

	case State::Fade: {
		float t = counter_ / fadeDuration_;
		if (t >= 1.0f) {
			t = 1.0f;
			state_ = State::Dead;
		}
		// t を使って alpha 値を外部に通知するなど必要なら
		alpha_ = 1.0f - t; // 時間に応じて徐々に透明に
		color.SetColor({1.0f, 1.0f, 1.0f, alpha_});

		break;
	}

	case State::Dead:
		// 何もしない or 即 return;
		return;
	}

	circleWorldTransform.matWorld_ = MakeAffineMatrix(circleWorldTransform.scale_, circleWorldTransform.rotation_, circleWorldTransform.translation_);
	circleWorldTransform.TransferMatrix();

	WorldTrnasformUpdate(circleWorldTransform);

	for (KamataEngine::WorldTransform& worldTransform : ellipseworldTransforms_) {

		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);

		worldTransform.TransferMatrix();

		WorldTrnasformUpdate(worldTransform);
	}
}

void HitEffect::Draw() {
	// モデルが設定されている場合のみ描画

	model_->Draw(circleWorldTransform, *camera_, &color);

	for (auto& worldTransform : ellipseworldTransforms_) {
		model_->Draw(worldTransform, *camera_, &color);
	}
}
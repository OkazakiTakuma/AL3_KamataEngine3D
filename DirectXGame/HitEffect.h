#pragma once
#include <KamataEngine.h>
#include "WorldTransformClass.h"
#include "Random.h"
class HitEffect {

	enum class State {
		Spread, // 拡大中
		Fade,   // フェード中
		Dead    // 終了
	};

public:

    

	void Initialize(KamataEngine::Vector3& spawnPosition);

	void Update();

	void Draw();

	static void SetModel(KamataEngine::Model* model) { model_ = model; }
	static void SetCamera(KamataEngine::Camera* camera) { camera_ = camera; }

	static HitEffect* Create(KamataEngine::Vector3& spawnPosition);

	bool IsDead() const { return state_ == State::Dead; }

private:
	// モデル
	static KamataEngine::Model* model_;
	// カメラ
	static KamataEngine::Camera* camera_;

	KamataEngine::WorldTransform circleWorldTransform;

	std::array<KamataEngine::WorldTransform, 4> ellipseworldTransforms_;

	float ellipseRotate;

	Random random_;

	int hitAnimeCount;

	State state_ = State::Spread;
	float counter_ = 0.0f;
	float spreadDuration_ = 0.3f; // スプレッド時間
	float fadeDuration_ = 0.3f;   // フェード時間
	float alpha_ = 1.0f;          // 描画用アルファ値（1:不透明 → 0:透明）
	KamataEngine::ObjectColor color;

};
#pragma once
#include "KamataEngine.h"


class Skydome {
public:
	void Initialize(const KamataEngine::Camera* camera);
	~Skydome();
	void Update();
	void Draw();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* modelSkydome_ = nullptr;
	// テクスチャハンドル
	uint32_t textstureHandle_ = 0u;
	// カメラ
	const KamataEngine::Camera* camera_ = nullptr;
};

#pragma once
#include "KamataEngine.h"

class GameScene {
public:
	void Initialize();
	~GameScene();
	void Update();
	void Draw();

private:
	// テクスチャーハンドル
	uint32_t tecstureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;
	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	// Imguiで値を入力する変数　
	float inputFloat3[3] = {0, 0, 0};
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	// 音楽再生ハンドル
	uint32_t voiceHandle_ = 0;
};

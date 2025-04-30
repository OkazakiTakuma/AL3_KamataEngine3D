#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include "Matrix.h"
#include <vector>


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
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// スカイドーム
	Skydome* skydome_ = nullptr;
	Vector3Matrix scale_;
	Vector3Matrix rotate_;
	Vector3Matrix translate_;
};

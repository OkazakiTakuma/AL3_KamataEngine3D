#pragma once
#include "KamataEngine.h"
#include "MapChipFIeld.h"
#include "Matrix.h"
#include "Player.h"
#include "Skydome.h"
#include "worldMatrix.h"
#include <vector>

class GameScene {
public:
	void Initialize();
	~GameScene();
	void Update();
	void Draw();
	void GenerateBlock();

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

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransFormBlocks_;

	Vector3Matrix scale_;
	Vector3Matrix rotate_;
	Vector3Matrix translate_;

	// スカイドーム
	Skydome* skydome_ = nullptr;
	// マップチップフィールド
	MapChipFIeld* mapChipField_ = nullptr;
	/// プレイヤー

	// プレイヤーの初期座標
	KamataEngine::Vector3 playerPosition_ = {0, 0, 0};
	// 自キャラ
	Player* player_ = nullptr;
};

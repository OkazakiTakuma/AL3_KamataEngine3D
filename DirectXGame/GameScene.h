#pragma once
#include "KamataEngine.h"
#include "MapChipFIeld.h"
#include "Matrix.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "worldMatrix.h"
#include "CameraController.h"
#include <vector>

class GameScene {
public:
	void Initialize();
	~GameScene();
	void Update();
	void Draw();
	void GenerateBlock();

private:
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

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

	KamataEngine::Vector3 scale_;
	KamataEngine::Vector3 rotate_;
	KamataEngine::Vector3 translate_;

	// スカイドーム
	Skydome* skydome_ = nullptr;
	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;
	// カメラコントローラー
	CameraController* cameraController_ = nullptr;
	/// プレイヤー

	// プレイヤーの初期座標
	KamataEngine::Vector3 playerPosition_ = {0, 0, 0};
	// 自キャラ
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	// 敵キャラ
	std::list<Enemy*> enemies_;
	// 敵キャラのモデル
	KamataEngine::Model* enemyModel_ = nullptr;
	// 敵キャラの初期座標

	void CheckALLCollision();
};

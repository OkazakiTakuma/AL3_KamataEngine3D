#pragma once
#include "KamataEngine.h"
#include "Player.h"

class GameScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// テクスチャーハンドル
	uint32_t playerTecstureHandle_ = 0;
	// スプライト
	KamataEngine::Sprite* playerSprite_ = nullptr;
	// 3Dモデル
	KamataEngine::Model* playerModel_ = nullptr;
	// ワールドトランスフォーム
	KamataEngine::WorldTransform playerWorldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	// 自キャラ
	Player* player_ = nullptr;
};

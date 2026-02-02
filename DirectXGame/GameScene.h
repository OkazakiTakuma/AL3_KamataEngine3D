#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Fade.h"
#include "KamataEngine.h"
#include "MapChipFIeld.h"
#include "Matrix.h"
#include "Player.h"
#include "Skydome.h"
#include "worldMatrix.h"
#include <vector>
#include <list>
#include<iostream>


class GameScene {
public:
	enum class Phase {
		kFadeIn,
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
		kFadeOut
	};
	void Initialize();
	~GameScene();
	void Update();
	void Draw();
	void GenerateBlock();
	void ChangePhase();
	void CheckALLCollision();
	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;
	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
	// テクスチャーハンドル
	uint32_t tecstureHandle_ = 0;
	uint32_t crackBlockTextureHandle_ = 0;
	// サウンドデータ
	uint32_t bgmHandle_ = 0;
	uint32_t bgmPlayHandle_ = 0;
	uint32_t sceneSeHandle_ = 0;
	uint32_t scenePlayHandle_ = 0;
	// スプライト
	std::unique_ptr<KamataEngine::Sprite> sprite_ = nullptr;
	// 3Dモデル
	std::unique_ptr<KamataEngine::Model> blockModel_ = nullptr;
	std::unique_ptr<KamataEngine::Model> ladderModel_ = nullptr;
	std::unique_ptr<KamataEngine::Model> goalModel_ = nullptr;
	std::unique_ptr<KamataEngine::Model> ropeModel_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	// デバッグカメラ
	std::unique_ptr<KamataEngine::DebugCamera> debugCamera_ = nullptr;

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
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<KamataEngine::Model> playerModel_ = nullptr;
	// 敵キャラ
	std::list<Enemy*> enemies_;
	// 敵キャラのモデル
	std::unique_ptr<KamataEngine::Model> enemyModel_ = nullptr;

	// デスパーティクル
	std::unique_ptr<DeathParticles> deathParticles_ = nullptr;
	// デスパーティクルのモデル
	std::unique_ptr<KamataEngine::Model> deathParticlesModel_ = nullptr;

	void UpdateBlocksTransforms();
	void UpdateEnemies();
	// GameScene.h のメンバ
	std::vector<Enemy*> pendingDeleteEnemies_;
	// 崩れる床タイマー管理用構造体
	struct CrackBlockTimer {
		uint32_t xIndex;
		uint32_t yIndex;
		float timer;
	};

	// GameScene クラスの private メンバに追加
	std::vector<CrackBlockTimer> crackBlockTimers;
};

#pragma once
#include "AABB.h"
#include "KamataEngine.h"
#include "WorldTransformClass.h"
#include "Easing.h"
#include "GameScene.h"
class Player;
class GameScene;

class Enemy {

public:
	// ふるまい
	enum class Behavior {
		kWalk,  // 通常状態
		kDeath, // 死亡状態
		kUnKnown, // 不明状態
	};

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	void WorldPosUpdate();

	AABB GetAABB();

	void OnCollision(const Player* player);

	bool IsDead() const { return isDead_; }

	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	GameScene* gameScene_; // ゲームシーンへのポインタ

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	Behavior behavior_ = Behavior::kWalk;

	// 次の振る舞いリクエスト
	Behavior behaviorRequest_ = Behavior::kUnKnown;

	uint32_t textureHandle_ = 0u;

	static inline const float kWalkSpeed = 0.1f;
	// 速度
	KamataEngine::Vector3 velostity_{};

	//  float pi = 0.0f; // float版π

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 30.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

	float turnNum = 0.0f;

	// デスフラグ
	bool isDead_ = false;

	// 死亡演出用タイマー
	float deathTimer_ = 0.0f;
	static inline const float kDeathTime = 1.0f; // 死亡アニメーションの長さ（秒）

	bool isCollisionDisabled_ = false; // 当たり判定無効化フラグ

};
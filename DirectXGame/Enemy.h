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

	void OnCollisionPlayer(Player* player);
	bool GetIsDead();


private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	static inline const float kWalkSpeed = 0.01f; // 敵キャラの歩行速度
	KamataEngine::Vector3 velocity_ = {0, 0, 0};  // 敵キャラの速度
	static inline const float kWorkMotionAngleStart = 0.0f; // 敵キャラのワークモーション開始角度
	static inline const float kWorkMotionAngleEnd = 3.14f;  // 敵キャラのワークモーション終了角度
	static inline const float kWarlMotionTime = 1.0f;       // 敵キャラのワークモーション時間
	float warkTimer_ = 0.0f;                                // 敵キャラのワークモーションタイマー
	bool isDead_ = false;

};


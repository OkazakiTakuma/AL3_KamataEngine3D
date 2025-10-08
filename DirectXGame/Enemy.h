#pragma once
#include "KamataEngine.h"
#include "Matrix.h"

class Player; // 前方宣言
class Enemy {
public:
	
	void Initialize(const KamataEngine::Vector3& position, KamataEngine::Model* model);
	void Update();
	void Draw(const KamataEngine::Camera* camera);

	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollisionPlayer(Player* player);
	bool GetIsDead();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	static inline const float kWalkSpeed = 0.01f; // 敵キャラの歩行速度
	KamataEngine::Vector3 velocity_ = {0, 0, 0};  // 敵キャラの速度
	static inline const float kWorkMotionAngleStart = 0.0f; // 敵キャラのワークモーション開始角度
	static inline const float kWorkMotionAngleEnd = 3.14f;  // 敵キャラのワークモーション終了角度
	static inline const float kWarlMotionTime = 1.0f;       // 敵キャラのワークモーション時間
	float warkTimer_ = 0.0f;                                // 敵キャラのワークモーションタイマー
	bool isDead_ = false;
};

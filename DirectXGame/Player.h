
#pragma once
#include "KamataEngine.h"
#include "Matrix.h"
#include <algorithm>
#include <assert.h>
#include <numbers>
#include<cmath>
#include"worldMatrix.h"
#include<imgui.h>
enum class LRDirection {
	kRight,
	kLeft,
};
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャーハンドル</param>
	/// <param name="camera">カメラ</param>
	void Initialize( const KamataEngine::Vector3& position);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const KamataEngine::Camera* camera);
	
const KamataEngine::WorldTransform &GetWorldTransform();
	const KamataEngine::Vector3& GetVelocoty() { return velocity_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textstureHandle_ = 0u;
	// カメラ
	const KamataEngine::Camera* camera_ = nullptr;
	// プレイヤーの初期速度
	KamataEngine::Vector3 velocity_ = {0, 0, 0};
	// プレイヤーの移動速度
	KamataEngine::Vector3 acceleration_ = {0, 0, 0};
	// プレイヤーの加速度
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunspeed = 0.5f;
	// プレイヤーの向き
	LRDirection lrDirection_ = LRDirection::kRight;
	// プレイヤーの旋回開始時の角度
	float turnFirstRotationY_ = 0;
	// 旋回タイマー
	float turnTimer_ = 0;
	// 旋回時間<秒>
	static inline const float kTimeturn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度(下)
	static inline const float kGravityAccleration = 0.01f;
	// 最大落下速度
	static inline const float kMaxFallSpeed = 1.0f;	
	// ジャンプ力
	static inline const float kJumpPower = 0.4f;
	// 
	float groundPostion_;
	Vector3Matrix scale_;
	Vector3Matrix rotate_;
	Vector3Matrix translate_;
	
};

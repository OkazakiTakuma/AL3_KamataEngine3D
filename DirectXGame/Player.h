
#pragma once
#include "KamataEngine.h"
#include "Matrix.h"
#include "worldMatrix.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <imgui.h>
#include <numbers>
enum class LRDirection {
	kRight,
	kLeft,
};
static inline const float kWidth = 0.8f*2;
static inline const float kHeight = 0.8f*2;
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner	  // 要素数
};

struct CollisionMapInfo {
	bool isCeilingCollision = false;
	bool isFloorCollision = false;
	bool isWallCollision = false;
	KamataEngine::Vector3 movement;
};

class MapChipField;
class Enemy;
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャーハンドル</param>
	void Initialize(const KamataEngine::Vector3& position, KamataEngine::Model* model);
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

	KamataEngine::Vector3 GetWorldPosition();
	AABB GetAABB();
	/// <summary>
	/// ワールドトランスフォームを参照
	/// </summary>
	/// <returns></returns>
	const KamataEngine::WorldTransform& GetWorldTransform();

	/// <summary>
	/// 速度を参照
	/// </summary>
	/// <returns></returns>
	const KamataEngine::Vector3& GetVelocity() { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	/// <summary>
	/// 判定結果を反映させて移動させる
	/// </summary>
	void Move(const CollisionMapInfo& info);

	void CollisionCeiling(CollisionMapInfo& info);

	void CollisionFloor(CollisionMapInfo& info);

	void CollisionWall(CollisionMapInfo& info);

	void OnCollisionEnemy(Enemy* enemy);

	bool IsDead() const { return isDead_; }

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textstureHandle_ = 0u;
	// カメラ
	const KamataEngine::Camera* camera_ = nullptr;
	// 生きているか
	bool isDead_ = false;
	// プレイヤーの初期速度
	KamataEngine::Vector3 velocity_ = {0, 0, 0};
	// プレイヤーの移動速度
	KamataEngine::Vector3 acceleration_ = {0, 0, 0};
	// プレイヤーの加速度
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kAttenuationWall = 0.5f; // 最大移動速度
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
	bool onGround_ = false;
	// 二段ジャンプ
	bool isSkyJump_ = false;
	// 重力加速度(下)
	static inline const float kGravityAccleration = 0.01f;
	// 最大落下速度
	static inline const float kMaxFallSpeed = 1.0f;
	// ジャンプ力
	static inline const float kJumpPower = 0.4f;
	//
	KamataEngine::Vector3 scale_ = {0};
	KamataEngine::Vector3 rotate_ = {0};
	KamataEngine::Vector3 translate_ = {0};
	MapChipField* mapChipField_ = nullptr;
	// キャラクターの当たり判定の大きさ
	static inline const float kBlank = 0.8f;

	void KeyMove();
	void ChengeOnGround(CollisionMapInfo& info);
	const float kOverGround = 0.6f; // 接地判定のオーバーグラウンド
} ;
void IsMapCollision(CollisionMapInfo& info, const KamataEngine::WorldTransform& worldTransform, MapChipField* mapChipField);
void IsTopCollision(CollisionMapInfo& info, const KamataEngine::WorldTransform& worldTransform_, MapChipField* mapChipField);
void IsBottomCollision(CollisionMapInfo& info, const KamataEngine::WorldTransform& worldTransform_, MapChipField* mapChipField);

void IsRightCollision(CollisionMapInfo& info, const KamataEngine::WorldTransform& worldTransform_, MapChipField* mapChipField);
void IsLeftCollision(CollisionMapInfo& info, const KamataEngine::WorldTransform& worldTransform_, MapChipField* mapChipField);

	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& centor, Corner corner);

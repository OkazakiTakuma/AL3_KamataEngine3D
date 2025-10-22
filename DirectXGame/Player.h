#pragma once
#include "AABB.h"
#include "KamataEngine.h"

#include "Matrix4x4_.h"
#include "WorldTransformClass.h"
#include <cassert>
#include "Easing.h"

// 02_14の29ページから再開

class MapChipField;

class Enemy;

/// <summary>
/// 自キャラ
/// </summary>

class Player {

public:
	enum class LRDirection {
		kRight,
		kLeft,
	};

	enum Corner {
		kRightBottom, // 右下
		kLeftBottom,  // 左下
		kRightTop,    // 右上
		kLeftTop,     // 左上

		kNumCenter // 要素数
	};

	// マップとの当たり判定情報
	struct ColisionMapInfo {
		bool celling = false;            // 天井
		bool landing = false;            // 床
		bool isWall = false;             // 壁
		KamataEngine::Vector3 velosity_; // 移動量
	};

	// ふるまい
	enum class Behavior {
		kRoot,    // 通常状態
		kAttak,   // 攻撃中
		kUnKnown, // 不明状態
	};

	// 攻撃フェーズ(型)
	enum class AttakFase {

		kChage,     // チャージ
		kAttack,    // 攻撃
		kAfterglow, // 余韻
		kUnKnown,   // 不明状態
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>

	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Model* attackModel, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void TitleUpdata();

	void PlayerMove();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw();


	const KamataEngine::WorldTransform& GetWoldTransform() const;

	void SetPosition(const KamataEngine::Vector3& position);

	const KamataEngine::Vector3& GetVelosity() const { return velosity_; }

	const KamataEngine::Vector3& GetPosition() const { return worldTransform_.translation_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void MapCollisionCheck(ColisionMapInfo& info);

	void MapCllisionCheckUp(Player::ColisionMapInfo& info);

	void MapCllisionCheckDown(Player::ColisionMapInfo& info);

	void MapCllisionCheckLeft(Player::ColisionMapInfo& info);

	void MapCllisionCheckRight(Player::ColisionMapInfo& info);

	void OnSwichGround(const Player::ColisionMapInfo& info);

	void IsCelling(Player::ColisionMapInfo& info);

	void IsWall(Player::ColisionMapInfo& info);

	void OnCollision(const Enemy* enemy);

	void TurnUpdata();

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	KamataEngine::WorldTransform worldTransform_;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	void MoveAccordingResult(const ColisionMapInfo& info);

	void WorldPosUpdate(const ColisionMapInfo& info);

	KamataEngine::Vector3 GetWorldPosition();

	AABB GetAABB();

	bool IsDead() const { return isDead_; }

	void BehaviorRootUpdate();

	void BehaviorAttackUpdate();

	// 通常行動初期化
	void BehaviorRootInitialize();

	// 攻撃行動初期化
	void BehaviorAttackInitialize();

	AttakFase attakFase_ = AttakFase::kUnKnown; // 攻撃フェーズ

	void ChageUpdate();

	void AttackUpdate();

	void AfterglowUpdate();

	bool IsAttack() const { return isAttack_; }

private:
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	uint32_t textureHandle_ = 0u;

	KamataEngine::Vector3 velosity_ = {};

	KamataEngine::Model* attackModel_ = nullptr;
	KamataEngine::WorldTransform worldTransformAttack_;

	static inline const float kAcceleration = 0.1f;

	static inline const float kAttenuation = 0.05f;

	static inline const float kLimitRunSpeed = 0.5f;


	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTurnTime = 0.3f;


	// 設置状態フラグ
	bool onGround_ = true;

	// 重力加速度
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 0.1f;
	// ジャンプ初速
	static inline const float kJumpAcceleration = 1.0f;
	static inline const float kBlank = 2.0f;
	static inline const float kLandingAdjust = 0.0001f; // 着地調整量
	static inline const float kAttenuationLanding = 0.1f;
	static inline const float smallNum = 0.05f;
	static inline const float kAttenuationWall = 0.01f;
	const float kEpsilon = 0.01f;
	// デスフラグ
	bool isDead_ = false;
	float rotateTimer_ = 0.0f;
	// ふるまい
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	Behavior behaviorRequest_ = Behavior::kUnKnown;
	// 攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;

	static inline const float kAttackTime = 20.0f; // 攻撃時間

	static inline const float chageTime = 10.0f; // チャージ時間

	static inline const float kAfterglowTime = 1.0f; // 余韻時間

	static inline const float attakVelosity = 0.8f; // 突進距離

	KamataEngine::WorldTransform correctionTransform; // マップチップの座標補正用

	//攻撃フラグ
	bool isAttack_ = false;

	bool isStepping_ = false; // 1マス移動中フラグ
	KamataEngine::Vector3 stepStart_{}, stepGoal_{};
	int stepFrame_ = 0;                      // 経過フレーム
	static inline const int kStepFrames = 6; // 1マスに要するフレーム数（速度調整用）

	bool TryStartStep(int dx, int dy); // ← 目的地チェック＆ステップ開始
	void UpdateStep();                 // ← ステップ中の補間更新

};


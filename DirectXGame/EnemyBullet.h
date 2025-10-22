#pragma once
#include "KamataEngine.h"
#include "WorldTransformClass.h"
#include "Operator.h"
#include "Easing.h"
#include "Collider.h"
#include "CollisionConfig.h"
class Player;

class EnemyBullet:public Collider {
public: 

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize( const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity, Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const KamataEngine::Camera& camera);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }

		// 衝突を検知したら曜日出されるコールバック関数
	void OnCollision();

	KamataEngine::Vector3 GetWorldPosition() const override { return worldTransform_.translation_; }


	KamataEngine::Vector3 GetPosition();

	
	float GetRadius() const { return 0.5f; } // 弾の半径（仮値）

		 static void LoadModel(); // モデルを事前に読み込む

	private:

	KamataEngine::WorldTransform worldTransform_;
	static KamataEngine::Model* model_;
	uint32_t textureHandle_ = 0;

	static const int32_t kLifeTime = 60 * 3;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	KamataEngine::Vector3 velocity_{}; // 追加：方向と速さを持つベクトル

	Player* player_ = nullptr; // 追加：プレイヤーへの参照


};


#pragma once
#include "KamataEngine.h"
#include "WorldTransformClass.h"
#include "Operator.h"
#include "Collider.h"
#include "CollisionConfig.h"

class PlayerBullet:public Collider {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize( const KamataEngine::Vector3& position,const KamataEngine::Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(const KamataEngine::Camera& camera);

	bool IsDead() const { return isDead_; }

	// 衝突を検知したら曜日出されるコールバック関数
	void OnCollision();

	KamataEngine::Vector3 GetWorldPosition() const override { return worldTransform_.translation_; }


	KamataEngine::Vector3 GetPositon() {
		KamataEngine::Vector3 worldPos;
		worldPos = worldTransform_.translation_;
		return worldPos;
	}

	float GetRadius() const { return 0.5f; }  

		static void LoadModel();

private:

	KamataEngine::WorldTransform worldTransform_;
	static KamataEngine::Model* model_;
	uint32_t textureHandle_ = 0;
	KamataEngine::Vector3 velosity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
#pragma once
#include "KamataEngine.h"
#include "Matrix.h"
#include "worldMatrix.h"
#include <array>
#include <numbers>

class DeathParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャーハンドル</param>
	void Initialize(const KamataEngine::Vector3& position, KamataEngine::Model* model);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const KamataEngine::Camera* camera);
	bool IsFinished() const { return finished_; }

private:
	static inline const uint32_t kNumParticles = 8;                                       // パーティクルの数
	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransform_;              // パーティクルのワールドトランスフォーム
	KamataEngine::Model* model_ = nullptr;                                                // モデル
	uint32_t textureHandle_ = 0u;                                                         // テクスチャハンドル
	static inline const float kDuration = 1.0f;                                           // パーティクルの持続時間
	static inline const float kSpeed = 0.1f;                                              // パーティクルの速度
	static inline const float kAngkeunit = 2 * std::numbers::pi_v<float> / kNumParticles; // パーティクルの角度単位
	bool finished_ = false;                                                               // パーティクルの終了フラグ
	float counter_ = 0.0f;                                                                // カウンター
	KamataEngine::ObjectColor objectColor_;                                               // オブジェクトの色
	KamataEngine::Vector4 color_;                                                         // パーティクルの色
};

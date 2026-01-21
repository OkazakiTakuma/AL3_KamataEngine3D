#pragma once
#include "KamataEngine.h"
#include "WorldTransformClass.h"
#include <array>

class DethParticles {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	void Draw();

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	bool IsFinished() const { return isFinished_; }

private:
	static inline const uint32_t kNumParticleSize = 8;

	std::array<KamataEngine::WorldTransform, kNumParticleSize> worldTransforms_;

	static inline const float PI = 3.1415926535897931f;

	// パーティクルの持続時間
	static inline const float kDuration = 1.0f;
	//移動の速さ
	static inline const float kSpeed = 0.1f; // パーティクルの移動速度
	//分割した一個分の角度
	static inline const float kAngleUint = (2*PI) / kNumParticleSize; // パーティクルの角度分割
	//終了フラグ
	bool isFinished_ = false;
	//時間経過カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	KamataEngine::ObjectColor objectColor_;
	//色の数値
	KamataEngine::Vector4 color_;

};

#pragma once
#include "KamataEngine.h"
#include "Matrix.h"
#include<cmath>
using namespace KamataEngine;

class Player {
public:
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャーハンドル</param>
	/// <param name="camera">カメラ</param>
	void Initialize(const KamataEngine::Camera* camera);

	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Update();
	/// <summary>
	/// デストラクタ
	/// </summary>

	void Draw();

private:


	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textstureHandle_ = 0u;
	// カメラ
	const KamataEngine::Camera* camera_ = nullptr;
	Vector3Matrix scale_;
	Vector3Matrix rotate_;
	Vector3Matrix translate_;
};
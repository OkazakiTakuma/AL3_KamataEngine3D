#pragma once
#include "KamataEngine.h"

class Player;

//矩形
struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;

};

class CameraController {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	void SetMovableArea(Rect area) { movableArea_ = area;}

private:
	// ビュープロジェクション
	KamataEngine::Camera* camera_=nullptr;
	Player* target_ = nullptr;
	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};
	Rect movableArea_ = {0, 100, 0, 100};
	KamataEngine::Vector3 goalPosition_ = {0, 0, 0};
	//座標補間割合
	static inline const float kInterpolationRate = 0.1f;
	//速度掛け率
	static inline const float kVelosityBias = 5.0f;
	
	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect marginArea_ = {-3.0f, 3.0f, -2.0f, 2.0f};

};
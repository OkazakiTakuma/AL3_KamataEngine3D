#pragma once
#pragma once
#include "KamataEngine.h"
#include "MapChipFIeld.h"
#include "Matrix.h"
#include "worldMatrix.h"
#include <vector>
#include <algorithm> 
#include<cmath>


struct Rect {
	float left = 0.0f;		// 左端
	float right = 1.0f;		// 右端
	float bottom = 0.0f;	// 下端
	float top = 1.0f;		// 上端
};

// 前方宣言
class Player;

/// <summary>
/// カメラコントローラ
/// </summary>
class CameraController {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CameraController() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 追従するターゲット
	/// </summary>
	void SetTarget(Player* player) { target_ = player; }

	void Reset();

	KamataEngine::Camera &GetCamera();

	void SetMovableArea(Rect area) { movableArea_ = area; };

private:
	KamataEngine::Camera camera_; // カメラ
	Player* target_ = nullptr;               // プレイヤー
	Vector3Matrix targetOffset_ = {0, 0, -30.0f};  // 追従対象とカメラの座標の差（オフセット）
	Matrix4x4Afifne viewProjection;
	Rect movableArea_;
	KamataEngine::Vector3 endCameraTranslate_; //カメラの目標座標
	static inline const float kInteroilationRote = 0.2f;
	static inline const float kVelocityBias = 30.0f;
	static inline const Rect margin = {-20, 20, -20, 20};
};

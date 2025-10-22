#pragma once
#include "KamataEngine.h"
#include "Title.h"
#include "Player.h"
#include "Fade.h"
class TitleScene {

public:

	enum class Phase {

		kFadeIn, // フェードイン
		kMain,   // メインフェーズ
		kFadeOut, // フェードアウト

	};

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	KamataEngine::Model* titleModel_ = nullptr; // タイトル用のモデル

	KamataEngine::Model* playerModel_ = nullptr; // プレイヤー用のモデル

	uint32_t playerTextureHandle_ = 0u; // プレイヤー用のテクスチャハンドル

	KamataEngine::Camera* camera_; // カメラ

	KamataEngine::WorldTransform worldTransform_; // ワールドトランスフォーム

	uint32_t textureHandle_ = 0u;

	Title* title_ ; // タイトル用のクラス

	Player* player_ ; // プレイヤー用のクラス

	bool IsFinished() const { return finished_; } // シーンが終了したかどうかを返す

	Fade* fade_ = nullptr;

	KamataEngine::Model* attackModel_ = nullptr; // 攻撃用のモデル


private:
	KamataEngine::Vector3 titlePosition_ ; // タイトルの位置

	KamataEngine::Vector3 playerPosition_ = {0.0f, 0.0f, 0.0f}; // プレイヤーの位置

	bool finished_ = false; // シーンの終了フラグ

	float duration_ = 1.0f;

	Phase phase_ = Phase::kFadeIn; // シーンのフェーズ

};

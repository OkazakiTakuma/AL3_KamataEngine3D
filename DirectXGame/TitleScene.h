#pragma once
#include <KamataEngine.h>
#include"Matrix.h"
#include"worldMatrix.h"
#include"Fade.h"
#include"Skydome.h"


/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	enum Phase {
		kFadeIn,
		kMain,
		kFadeOut,

	};
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	// スカイドーム
	Skydome* skydome_ = nullptr;
	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Sprite* serectSprite_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t titleTextureHandle_ = 0;
	uint32_t bgmHandle_ = 0;
	uint32_t bgmPlayHandle_ = 0;
	uint32_t sceneSeHandle_ = 0;
	uint32_t scenePlayHandle_ = 0;
	bool finished_=false;
	KamataEngine::WorldTransform  worldTransform_;
	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Camera camera_;
	float timer_ = 0.0f;
	float addtimer = 1.0f / 60.0f;  
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kMain;
};

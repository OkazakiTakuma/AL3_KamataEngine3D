#pragma once
#include <KamataEngine.h>
#include"Matrix.h"
#include"worldMatrix.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();
	bool IsFinished() const { return finished_; }

private:
	bool finished_=false;
	KamataEngine::WorldTransform  worldTransform_;
	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Camera camera_;
	float timer_ = 0.0f;
	float addtimer = 1.0f / 60.0f;
};

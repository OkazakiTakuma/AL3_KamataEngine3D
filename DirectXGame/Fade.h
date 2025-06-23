#pragma once
#include "Matrix.h"
#include "worldMatrix.h"
#include <KamataEngine.h>

class Fade {

public:
	enum  Status {
		None,
		FadeIn,
		FadeOut,

	};
	void Initialize();
	void Update();
	void Draw();
	void Start(Status status, float duration);
	void Stop();
	bool IsFinished() const;

private:
	uint32_t fadeTexture = 0u;
	KamataEngine::Sprite* sprite_ = nullptr;
	Status status_ = Status::None;
	float duration_ = 0.0f;
	float counter_ = 0.0f;
};

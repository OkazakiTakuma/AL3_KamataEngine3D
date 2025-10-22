#pragma once
#include "KamataEngine.h"
#include "WorldTransformClass.h"
#include "Matrix4x4_.h"
class Title {

public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	float floatTimer_ = 0.0f; 

	KamataEngine::Vector3 basePosition_;

	uint32_t textureHandle_ = 0u;
};

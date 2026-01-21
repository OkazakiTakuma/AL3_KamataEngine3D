#include "Skydome.h"

using namespace KamataEngine;
void Skydome::Initialize(const  Camera* camera) {
	modelSkydome_ = Model::CreateFromOBJ("skydome",true );
	camera_ = camera;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
}

Skydome::~Skydome() { delete modelSkydome_; }

void Skydome::Update() {}

void Skydome::Draw() { modelSkydome_->Draw(worldTransform_, *camera_); }

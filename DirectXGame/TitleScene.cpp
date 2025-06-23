#include "TitleScene.h"
using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete titleModel_;
	delete fade_;
}

void TitleScene::Initialize() {
	titleModel_ = Model::CreateFromOBJ("title", true);
	worldTransform_.Initialize();
	camera_.Initialize();
	camera_.translation_.z = -10;
	camera_.UpdateMatrix();
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::FadeIn, 1.0f);
	phase_ = Phase::kFadeIn;
}

void TitleScene::Update() {
	if (phase_ != Phase::kMain) {
		fade_->Update();
		if (fade_->IsFinished()) {
			if (phase_ == Phase::kFadeOut) {
				finished_ = true;
				return;
			}
			phase_ = Phase::kMain;
		}
	} else {
#pragma region シーン移行
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::FadeOut, 1.0f);

			// シーン移行のロジックをここに記述
		}
		timer_ += addtimer;
		if (timer_ > 1.0f) {
			addtimer *= -1;
		}
		if (timer_ < 0.0f) {
			addtimer *= -1;
		}
		worldTransform_.translation_ = Leap(Vector3(0, 0, 0), Vector3(0, 1, 0), timer_);
		WorldTransformUpdate(worldTransform_);
#pragma endregion
	}
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデルの描画
	Model::PreDraw(dxCommon->GetCommandList());
	titleModel_->Draw(worldTransform_, camera_);
	Model::PostDraw();
	if (phase_ != Phase::kMain) {

		fade_->Draw();
	}
}

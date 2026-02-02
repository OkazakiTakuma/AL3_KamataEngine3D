#include "TitleScene.h"
using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete titleModel_;
	delete fade_;
	delete skydome_;
	
}

void TitleScene::Initialize() {
	worldTransform_.Initialize();
	camera_.Initialize();
	camera_.translation_.z = -10;
	camera_.UpdateMatrix();
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::FadeIn, 1.0f);
	phase_ = Phase::kFadeIn;
	skydome_ = new Skydome();
	skydome_->Initialize(&camera_);
	bgmHandle_ = Audio::GetInstance()->LoadWave("audio/bgm/Titlebgm.wav");
	bgmPlayHandle_ = Audio::GetInstance()->PlayWave(bgmHandle_, true, 0.2f);
	sceneSeHandle_ = Audio::GetInstance()->LoadWave("audio/SE/sceneChange1.wav");
	textureHandle_ = TextureManager::Load("Title/title.png");
	sprite_ = Sprite::Create(textureHandle_, {450, 50});
	titleTextureHandle_ = TextureManager::Load("Title/UI.png");
	serectSprite_ = Sprite::Create(titleTextureHandle_, {430, 400});
}

void TitleScene::Update() {
	skydome_->Update();

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
			if (Audio::GetInstance()->IsPlaying(bgmPlayHandle_)) {
				Audio::GetInstance()->StopWave(bgmPlayHandle_);
			}
			scenePlayHandle_ = Audio::GetInstance()->PlayWave(sceneSeHandle_, false, 0.2f);
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
	skydome_->Draw();
	Model::PostDraw();
	Sprite::PreDraw(dxCommon->GetCommandList());
	serectSprite_->Draw();

	sprite_->Draw();
	Sprite::PostDraw();
	if (phase_ != Phase::kMain) {

		fade_->Draw();
	}
}

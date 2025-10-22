#include "Fade.h"
#include <algorithm>
void Fade::Initialize() {

	textureHandle_ = KamataEngine::TextureManager::Load("sample.png");

	// スプライトの初期化
	sprite_ = KamataEngine::Sprite::Create(textureHandle_, {0, 0});

	sprite_->SetSize(KamataEngine::Vector2(1280, 720)); // スプライトのサイズを画面サイズに設定
	sprite_->SetColor(KamataEngine::Vector4(0, 0, 0, 1.0f));
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	if (status_ == Status::FadeIn) {
		counter_ = duration_; // 最初は真っ黒 → 透明へ
	} else if (status_ == Status::FadeOut) {
		counter_ = 0.0f; // 最初は透明 → 真っ黒へ
	}

	  duration_ = duration; // ✅ duration_ を最後にセット（これ重要）
}

void Fade::Update() {
	// 更新処理
	// ここでは特に何もしない

	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:

		counter_ -= 1.0f / 60.0f;
		if (counter_ <= 0.0f) {
			counter_ = 0.0f;
		}

		sprite_->SetColor(KamataEngine::Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f))); // フェードアウトの色を設定

		break;
	case Status::FadeOut:

		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		sprite_->SetColor(KamataEngine::Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f))); // フェードアウトの色を設定

		break;
	default:
		break;
	}
}

void Fade::Stop() {

	status_ = Status::None; // フェードの状態をNoneに設定
}

bool Fade::IsFinished() const {

	switch (status_) {
	case Fade::Status::None:
	    break;
	case Fade::Status::FadeIn:
	    break;
	case Fade::Status::FadeOut:

	    if (counter_ >= duration_) {
	        return true;
	    } else {
	        return false;

	    }

	    break;

	}

	return true;
	
}
void Fade::Draw() {

	if (status_ == Status::None) {
		return;
	}

	KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();

	KamataEngine::Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	KamataEngine::Sprite::PostDraw();
}
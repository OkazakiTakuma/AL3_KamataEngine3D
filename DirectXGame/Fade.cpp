#include "Fade.h"
using namespace KamataEngine;

void Fade::Initialize() {
	fadeTexture = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(fadeTexture, {0, 0});
	sprite_->SetSize(Vector2(1280, 720));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() {
	switch (status_) {
	case Fade::None:
		break;
	case Fade::FadeIn:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		sprite_->SetColor(Vector4(0, 0, 0,std::clamp(1.0f - (counter_ / duration_), 0.0f, 1.0f)));
		break;
	case Fade::FadeOut:
		counter_ += 1.0f / 60.0f;
		if (counter_ >= duration_) {
			counter_ = duration_;
		}

		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));

		break;
	default:
		break;
	}
}

void Fade::Draw() {
	if (status_ == Status::None) {
		return;
	}
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const { 
	switch (status_) {
	case Fade::None:
		break;
	case Fade::FadeIn:
		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
		break;
	case Fade::FadeOut:
		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
		break;
	default:
		break;
	}
	return false; 
}

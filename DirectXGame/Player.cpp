
#include "Player.h"

using namespace KamataEngine;

void Player::Initialize( const Camera* camera, const Vector3& position) {
	model_ = Model::CreateFromOBJ("player", true);
	camera_ = camera;
	groundPostion_ = position.y;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2;
}

Player::~Player() {}

void Player::Update() {
	// プレイヤーの移動


	// 接地状態の時
	if (onGround_) {
		// 左右移動
#pragma region 左右移動
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中はブレーキ
				if (velocity_.x < 0) {
					acceleration_.x = 0;
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration_.x += kAcceleration;

				// 向きを変える
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeturn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中はブレーキ
				if (velocity_.x > 0) {
					acceleration_.x = 0;
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration_.x -= kAcceleration;
				// 向きを変える
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeturn;
				}
			}
			velocity_.x += acceleration_.x;
			// 速度の制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunspeed, kLimitRunspeed);

		} else {
			// 速度が減速する
			velocity_.x *= (1.0f - kAttenuation);
		}
#pragma endregion
		// ジャンプ
#pragma region ジャンプ
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_.y += kJumpPower;
			onGround_ = false;
		}
	} else {
		// 接地フラグ
		bool landing = false;
		//地面との判定
		// 下降中か
		if (velocity_.y < 0) {
		// Yが地面以下になったら
			if (worldTransform_.translation_.y <= groundPostion_) {
				// 接地フラグを立てる
				landing = true;
				// Y座標を0にする
			}
		}

	// 空中にいる時に
		// 重力をかける
		velocity_.y -= kGravityAccleration;
		// 速度の制限
		velocity_.y = std::clamp(velocity_.y, -kMaxFallSpeed, kMaxFallSpeed);

		if (landing) {
			// 接地フラグを立てる
			onGround_ = true;
			// Y座標を0にする
			worldTransform_.translation_.y = groundPostion_;
			// 摩擦
			velocity_.x *= (1.0f - kAttenuation);
			// 速度を0にする
			velocity_.y = 0;
		}
	}
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 旋回制御
	if (turnTimer_ > 0) {
		// 旋回中
		turnTimer_ -= 1.0f / 60.0f;
		// 左右のキャラの角度テーブル
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2,     // 右
		    std::numbers::pi_v<float> * 3 / 2, // 左
		};
		// 状況による向きの変更
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// イーズインアウトを使って徐々に向きの変更
		float t = turnTimer_ / kTimeturn;
		// 旋回角度
		float rotationY = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * (1.0f - t * t);
		worldTransform_.rotation_.y = rotationY;
	}
	// ジャンプ移動
	// 接地状態の時

	// ワールドトランスフォームの更新
	WorldTransformUpdate(worldTransform_);
	
}

void Player::Draw() {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_, textstureHandle_, nullptr);
}

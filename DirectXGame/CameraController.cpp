#include "CameraController.h"
#include "Player.h"
using namespace KamataEngine;

void CameraController::Initialize() { camera_.Initialize(); }

void CameraController::Update() {
	// 追従対象のワールドトランスフォームの取得
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を求める
	endCameraTranslate_.x = targetWorldTransform.translation_.x + targetOffset_.x + target_->GetVelocity().x * kVelocityBias;
	endCameraTranslate_.y = targetWorldTransform.translation_.y + targetOffset_.y + target_->GetVelocity().y * kVelocityBias;
	endCameraTranslate_.z = targetWorldTransform.translation_.z + targetOffset_.z + target_->GetVelocity().z * kVelocityBias;
	// 線形補間
	camera_.translation_ = Leap(camera_.translation_, endCameraTranslate_, kInteroilationRote);

	// 追従対象が画面から出ないように
	camera_.translation_.x = std::clamp(camera_.translation_.x, targetWorldTransform.translation_.x + margin.left, targetWorldTransform.translation_.x + margin.right);
	camera_.translation_.y = std::clamp(camera_.translation_.y, targetWorldTransform.translation_.y + margin.bottom, targetWorldTransform.translation_.y + margin.top);

	// 移動範囲の制限
	camera_.translation_.x = std::clamp(camera_.translation_.x, movableArea_.left, movableArea_.right);
	camera_.translation_.y = std::clamp(camera_.translation_.y, movableArea_.bottom, movableArea_.top);
	// 行列の更新

	camera_.UpdateMatrix();
}

void CameraController::Reset() {
	if (target_ != nullptr) {
		const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
		// 追従対象とオフセットからカメラの座標を求める
		camera_.translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
		camera_.translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
		camera_.translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

		// カメラ行列を更新
		camera_.UpdateMatrix();
	}
}

Camera& CameraController::GetCamera() { return camera_; }

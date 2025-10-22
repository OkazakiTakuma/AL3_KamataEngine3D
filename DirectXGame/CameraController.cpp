#define NOMINMAX
#include "CameraController.h"
#include "Player.h"
#include <algorithm>
void CameraController::Initialize(KamataEngine::Camera* camera) { camera_ = camera; }
KamataEngine::Vector3 Lerp(KamataEngine::Vector3 a, KamataEngine::Vector3 b, float t) { return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t}; }

void CameraController::Update() {
	// カメラの更新処理
	// 追従対象のワールドトランスフォームを参照
	const KamataEngine::WorldTransform& targetWorldTransform = target_->GetWoldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	// camera_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	// camera_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	// camera_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	

	goalPosition_.x = targetWorldTransform.translation_.x + targetOffset_.x + target_->GetVelosity().x + kVelosityBias;
	goalPosition_.y = targetWorldTransform.translation_.y + targetOffset_.y ;
	goalPosition_.z = targetWorldTransform.translation_.z + targetOffset_.z - target_->GetVelosity().z - kVelosityBias;


	

	camera_->translation_ = Lerp(camera_->translation_, goalPosition_, kInterpolationRate);

	// 追従対象が画面外に出ないように補正
	camera_->translation_.x = std::max(camera_->translation_.x, target_->GetPosition().x + marginArea_.left);
	camera_->translation_.x = std::min(camera_->translation_.x, target_->GetPosition().x + marginArea_.right);
	camera_->translation_.y = std::max(camera_->translation_.y, target_->GetPosition().y + marginArea_.bottom);
	camera_->translation_.y = std::min(camera_->translation_.y, target_->GetPosition().y + marginArea_.top);
	


	// 移動範囲制限
	camera_->translation_.x = std::clamp(camera_->translation_.x, movableArea_.left, movableArea_.right);
	camera_->translation_.y = std::clamp(camera_->translation_.y, movableArea_.bottom, movableArea_.top);

	// 行列を更新する
	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	// カメラのリセット処理
}
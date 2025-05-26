#include "Player.h"
#include "MapChipField.h"
#include <algorithm>
using namespace KamataEngine;

void Player::Initialize(const Vector3& position) {
	model_ = Model::CreateFromOBJ("player", true);
	groundPostion_ = position.y;
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2;
}

Player::~Player() {}

void Player::Update() {
	// プレイヤーの移動
	KeyMove();
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.movement = velocity_;
	IsMapCollision(collisionMapInfo, worldTransform_, mapChipField_);
	Move(collisionMapInfo);
	CollisonCeiling(collisionMapInfo);
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

void Player::Draw(const Camera* camera) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera, textstureHandle_, nullptr);
}

const KamataEngine::WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::Move(const CollisionMapInfo& info) {
	worldTransform_.translation_.x += info.movement.x;
	worldTransform_.translation_.y += info.movement.y;
	worldTransform_.translation_.z += info.movement.z;
}

void Player::CollisonCeiling(CollisionMapInfo& info) {
	if (info.isCeilingCollision) {
		// 天井に衝突した場合
		// 速度を0にする
		velocity_.y = 0;
	}
}
void Player::KeyMove() {
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
		// 地面との判定
		//  下降中か
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
}

void IsMapCollision(CollisionMapInfo& info, const WorldTransform& worldTransform, MapChipField* mapChipField) {
	IsTopCollision(info, worldTransform, mapChipField);
	IsBottomCollision(info);
	IsRightCollision(info);
	IsRightCollision(info);
}

void IsTopCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; i++) {
		// "worldTransform_" をクラスメンバーとしてアクセスするために "this->" を追加
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}
	if (info.movement.y <= 0) {
		return;
	}
	MapChipType mapChipType;

	bool hit = false;
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {

		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex);
		// 上に衝突した場合
		info.movement.y = static_cast<float>(rect.bottom) - positionsNew[kLeftTop].y-1;
		if (info.movement.y < 0) {
			info.movement.y = 0;
		}
		info.isCeilingCollision = true;
	}
}

void IsBottomCollision(CollisionMapInfo& info) { info; };

void IsRightCollision(CollisionMapInfo& info) { info; };

void IsLeftCollision(CollisionMapInfo& info) { info; };

Vector3 CornerPosition(const Vector3& centor, Corner corner) {

	Vector3 offsetTable[kNumCorner]{
	    {kWidth / 2.0f,  centor.y - kHeight / 2.0f, 0},
        {-kWidth / 2.0f, centor.y - kHeight / 2.0f, 0},
        {kWidth / 2.0f,  centor.y + kHeight / 2.0f, 0},
        {-kWidth / 2.0f, centor.y + kHeight / 2.0f, 0}
    };

	return {centor.x + offsetTable[static_cast<uint32_t>(corner)].x, centor.y + offsetTable[static_cast<uint32_t>(corner)].y, centor.z + offsetTable[static_cast<uint32_t>(corner)].z};
}
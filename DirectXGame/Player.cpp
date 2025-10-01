#include "Player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include <algorithm>
#include <iostream>

using namespace KamataEngine;

void Player::Initialize(const Vector3& position, Model* model) {
	model_ = model;
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
	CollisionCeiling(collisionMapInfo);
	CollisionFloor(collisionMapInfo);
	// 接地状態の変更
	ChengeOnGround(collisionMapInfo);
	CollisionWall(collisionMapInfo);

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

Vector3 Player::GetWorldPosition() {
	Vector3 worldPosition;
	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];
	return worldPosition;
}

AABB Player::GetAABB() {
	// AABBの計算
	AABB aabb;
	Vector3 worldPosition = GetWorldPosition();
	worldPosition.y *= 2;
	aabb.min = worldPosition - Vector3(kWidth / 2, kHeight / 2, kWidth / 2);
	aabb.max = worldPosition + Vector3(kWidth / 2, kHeight / 2, kWidth / 2);

	return aabb;
}

const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::Move(const CollisionMapInfo& info) {
	velocity_.x = info.movement.x;
	velocity_.y = info.movement.y;
	velocity_.z = info.movement.z;
}

void Player::CollisionCeiling(CollisionMapInfo& info) {
	if (info.isCeilingCollision) {
		// 天井に衝突した場合
		// 速度を0にする
		velocity_.y = 0;
	}
}

void Player::CollisionFloor(CollisionMapInfo& info) {
	if (info.isFloorCollision) {
		// 天井に衝突した場合
		// 速度を0にする
		velocity_.y = 0;
		onGround_ = true; // 接地フラグを立てる
	}
}

void Player::CollisionWall(CollisionMapInfo& info) {
	// 壁に衝突した場合
	if (info.isWallCollision) {
		velocity_.x *= (1.0f - kAttenuationWall); // 壁に衝突した場合は摩擦をかける
	}
}

void Player::OnCollisionEnemy(Enemy* enemy) {
	(void)enemy;
	isDead_ = true;
}

void Player::KeyMove() {
	// 接地状態の時
	// 左右移動
#pragma region 左右移動
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
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
		} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {
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
	if (onGround_) {
		isSkyJump_ = true;

		// ジャンプ
#pragma region ジャンプ
		if (Input::GetInstance()->TriggerKey(DIK_UP) || Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerKey(DIK_W)) {
			velocity_.y = 0.0f;
			velocity_.y += kJumpPower;
			onGround_ = false;
		}
	} else {
		if (isSkyJump_ == true) {
			if (Input::GetInstance()->TriggerKey(DIK_UP) || Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerKey(DIK_W)) {
				velocity_.y += kJumpPower;
				isSkyJump_ = false;
			}
		}
		// 空中にいる時に
		// 重力をかける
		velocity_.y -= kGravityAccleration;
		// 速度の制限
		velocity_.y = std::clamp(velocity_.y, -kMaxFallSpeed, kMaxFallSpeed);
	}
}

void Player::ChengeOnGround(CollisionMapInfo& info) {

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false; // 接地フラグを下ろす
		}
		bool hit = false;
		// 今空中にいないか
		Vector3 leftBottomPosition = CornerPosition(worldTransform_.translation_, Corner::kLeftBottom);
		leftBottomPosition.y -= kOverGround; // 接地判定のオーバーグラウンドを追加
		Vector3 rightBottomPosition = CornerPosition(worldTransform_.translation_, Corner::kRightBottom);
		rightBottomPosition.y -= kOverGround; // 接地判定のオーバーグラウンドを追加

		// マップチップの位置を取得
		MapChipField::IndexSet leftIndexSet = mapChipField_->GetMapChipIndexByPosition(leftBottomPosition);
		MapChipField::IndexSet rightIndexSet = mapChipField_->GetMapChipIndexByPosition(rightBottomPosition);
		// マップチップの位置から地面の高さを取得
		MapChipType leftMapChipType = mapChipField_->GetMapChipTypeIndex(leftIndexSet.xIndex, leftIndexSet.yIndex);
		MapChipType rightMapChipType = mapChipField_->GetMapChipTypeIndex(rightIndexSet.xIndex, rightIndexSet.yIndex);

		if (leftMapChipType == MapChipType::kBlock || rightMapChipType == MapChipType::kBlock) {
			hit = true;
		} else {
			hit = false;
		}
		if (!hit) {
			onGround_ = false;
		}
	} else {
		if (info.isFloorCollision && worldTransform_.translation_.y <= CornerPosition(worldTransform_.translation_, kLeftBottom).y) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0;
		}
	}
}

void IsMapCollision(CollisionMapInfo& info, const WorldTransform& worldTransform, MapChipField* mapChipField) {
	IsTopCollision(info, worldTransform, mapChipField);
	IsBottomCollision(info, worldTransform, mapChipField);
	IsRightCollision(info, worldTransform, mapChipField);
	IsLeftCollision(info, worldTransform, mapChipField);
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
	indexSet.xIndex;
	indexSet.yIndex;
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
		info.movement.y = static_cast<float>(rect.bottom) - 3 - kHeight;
		if (info.movement.y < 0) {
			info.movement.y = 0;
		}
		info.isCeilingCollision = true;
	}
}

void IsBottomCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; i++) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.y >= 0) {
		return; // 下降していない場合は判定不要
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左下のマップチップ判定
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下のマップチップ判定
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex - 1);
		info.movement.y = static_cast<float>(rect.top) + kHeight + 1;
		if (info.movement.y > 0) {
			info.movement.y = 0;
		}

		info.isFloorCollision = true; // 接地フラグ
	}
}
void IsRightCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; i++) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.x <= 0) {
		return; // 左へ移動中の場合は判定不要
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 右上のマップチップ判定
	positionsNew[kRightTop].x -= 0.5f; // 右上の位置を少し下にずらす
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 右下のマップチップ判定
	positionsNew[kRightBottom].x -= 0.5f; // 右下の位置を少し下にずらす
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex);

		// 右方向の衝突処理
		info.movement.x = static_cast<float>(rect.left) - 1; // 右方向
		if (info.movement.x > 0) {
			info.movement.x = 0;
		}
		info.isWallCollision = true;
	}
}
void IsLeftCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; i++) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.x >= 0) {
		return; // 右へ移動中の場合は判定不要
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;
	positionsNew[kLeftTop].x -= 0.6f;    // 左上の位置を少し下にずらす
	positionsNew[kLeftBottom].x -= 0.6f; // 左下の位置を少し下にずらす
	// 左上のマップチップ判定
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// 左下のマップチップ判定
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex);

		// 左方向の衝突処理
		info.movement.x = static_cast<float>(rect.right) + 1; // 左方向

		if (info.movement.x > 0) {
			info.movement.x = 0;
		}
		info.isWallCollision = true;
	}
}
Vector3 CornerPosition(const Vector3& centor, Corner corner) {

	Vector3 offsetTable[kNumCorner]{
	    {kWidth / 2.0f,  -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {kWidth / 2.0f,  +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };

	return {(centor.x + offsetTable[static_cast<uint32_t>(corner)].x) / 2, (centor.y + offsetTable[static_cast<uint32_t>(corner)].y) / 2, centor.z + offsetTable[static_cast<uint32_t>(corner)].z};
}
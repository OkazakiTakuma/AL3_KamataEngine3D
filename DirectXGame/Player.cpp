// Player.cpp — 改良実装（CornerPosition の /2 を維持）
#include "Player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(const Vector3& position, Model* model) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

Player::~Player() = default;

void Player::Update() {
	if (behaviorRequest_ != Behavior::kNull) {
		behavior_ = behaviorRequest_;
		switch (behavior_) {
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		}
		behaviorRequest_ = Behavior::kNull;
	}

	switch (behavior_) {
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	}

	WorldTransformUpdate(worldTransform_);
}

void Player::Draw(const Camera* camera) {
	// 3Dモデルを描画
	if (model_ && camera) {
		model_->Draw(worldTransform_, *camera, textstureHandle_, nullptr);
	}

	// ヒットラインを Draw のタイミングで表示する
	if (isDrawHitLine_) {
		// プレイヤーのワールド座標（ワールド行列の 4 列目）
		Vector3 playerPos;
		playerPos.x = worldTransform_.matWorld_.m[3][0];
		playerPos.y = worldTransform_.matWorld_.m[3][1];
		playerPos.z = worldTransform_.matWorld_.m[3][2];

		// PrimitiveDrawer で線を引く
		if (auto drawer = PrimitiveDrawer::GetInstance()) {
			drawer->DrawLine3d(playerPos, hitLineTarget_, hitLineColor_);
		}

		// タイマーを減らす。Draw はフレーム単位なのでフレームレート想定でデルタを減算
		// フレームレートが可変の場合は本来 Update 側で delta を渡す方が良いが
		// 要求どおり Draw タイミングで処理する（60FPS 想定）
		constexpr float kFrameDelta = 1.0f / 60.0f;
		hitLineTimer_ -= kFrameDelta;
		if (hitLineTimer_ <= 0.0f) {
			isDrawHitLine_ = false;
			hitLineTimer_ = 0.0f;
		}
	}
}

void Player::BehaviorRootInitialize() {}

void Player::BehaviorRootUpdate() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		behaviorRequest_ = Behavior::kAttack;
	}

	KeyMove();

	CollisionMapInfo collisionMapInfo{};
	collisionMapInfo.movement = velocity_;

	IsMapCollision(collisionMapInfo, worldTransform_, mapChipField_);

	Move(collisionMapInfo);
	CollisionCeiling(collisionMapInfo);
	CollisionFloor(collisionMapInfo);
	ChengeOnGround(collisionMapInfo);
	CollisionWall(collisionMapInfo);

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	if (turnTimer_ > 0.0f) {
		constexpr float kFrameDelta = 1.0f / 60.0f;
		turnTimer_ = std::max<float>(0.0f, turnTimer_ - kFrameDelta);
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		const uint32_t idx = static_cast<uint32_t>(lrDirection_);
		const float destinationRotationY = destinationRotationYTable[idx];
		const float t = turnTimer_ / kTimeturn;
		const float eased = 1.0f - t * t;
		worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * eased;
	}
}

void Player::BehaviorAttackInitialize() {}

void Player::BehaviorAttackUpdate() {
	isAttack_ = true;
	if (isAttackHit_) {
		isAttackHit_ = false;
		AttackHitUpdate();
		return;
	}
	behaviorRequest_ = Behavior::kRoot;
}

// Player.cpp 内の AttackHitUpdate を次のように置き換えてください。
void Player::AttackHitUpdate() {
	// プレイヤーのワールド位置は Draw 時に取得して描画するため、
	// ここではターゲット位置とタイマー／フラグだけ設定する
	hitLineTarget_ = targetWorldPotion_;
	isDrawHitLine_ = true;
	hitLineTimer_ = hitLineDuration_;

	// 攻撃処理が終わったので攻撃フラグは解除する
	isAttack_ = false;
}
void Player::SetTargetWorldPosition(const KamataEngine::Vector3& targetWorldPotion) { targetWorldPotion_ = targetWorldPotion; }

float Player::GetMaxAttackRange() { return maxAttackRange; }

bool Player::GetIsAttack() { return isAttack_; }

Vector3 Player::GetWorldPosition() {
	Vector3 worldPosition;
	worldPosition.x = worldTransform_.matWorld_.m[3][0];
	worldPosition.y = worldTransform_.matWorld_.m[3][1];
	worldPosition.z = worldTransform_.matWorld_.m[3][2];
	return worldPosition;
}

AABB Player::GetAABB() {
	AABB aabb;
	Vector3 worldPosition = GetWorldPosition();
	worldPosition.y *= 2.0f;
	aabb.min = worldPosition - Vector3(kWidth / 2.0f, kHeight / 2.0f, kWidth / 2.0f);
	aabb.max = worldPosition + Vector3(kWidth / 2.0f, kHeight / 2.0f, kWidth / 2.0f);
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
		velocity_.y = 0.0f;
	}
}

void Player::CollisionFloor(CollisionMapInfo& info) {
	if (info.isFloorCollision) {
		velocity_.y = 0.0f;
		onGround_ = true;
	}
}

void Player::CollisionWall(CollisionMapInfo& info) {
	if (info.isWallCollision) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::OnCollisionEnemy(Enemy* /*enemy*/) { isDead_ = true; }

void Player::KeyMove() {
	const Input* in = Input::GetInstance();
	bool pressingLeft = in->PushKey(DIK_LEFT) || in->PushKey(DIK_A);
	bool pressingRight = in->PushKey(DIK_RIGHT) || in->PushKey(DIK_D);

	if (pressingLeft || pressingRight) {
		if (pressingRight) {
			if (velocity_.x < 0.0f) {
				acceleration_.x = 0.0f;
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration_.x += kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeturn;
			}
		} else {
			if (velocity_.x > 0.0f) {
				acceleration_.x = 0.0f;
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration_.x -= kAcceleration;
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeturn;
			}
		}
		velocity_.x += acceleration_.x;
		velocity_.x = std::clamp(velocity_.x, -kLimitRunspeed, kLimitRunspeed);
	} else {
		velocity_.x *= (1.0f - kAttenuation);
		acceleration_.x = 0.0f;
	}

	if (onGround_) {
		isSkyJump_ = true;
		if (in->TriggerKey(DIK_UP) || in->TriggerKey(DIK_W)) {
			velocity_.y = kJumpPower;
			onGround_ = false;
		}
	} else {
		if (isSkyJump_) {
			if (in->TriggerKey(DIK_UP) || in->TriggerKey(DIK_W)) {
				velocity_.y += kJumpPower;
				isSkyJump_ = false;
			}
		}
		velocity_.y -= kGravityAccleration;
		velocity_.y = std::clamp(velocity_.y, -kMaxFallSpeed, kMaxFallSpeed);
	}
}

void Player::ChengeOnGround(CollisionMapInfo& info) {
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
			return;
		}
		bool hit = false;
		Vector3 leftBottomPosition = CornerPosition(worldTransform_.translation_, Corner::kLeftBottom);
		leftBottomPosition.y -= kOverGround;
		Vector3 rightBottomPosition = CornerPosition(worldTransform_.translation_, Corner::kRightBottom);
		rightBottomPosition.y -= kOverGround;

		MapChipField::IndexSet leftIndexSet = mapChipField_->GetMapChipIndexByPosition(leftBottomPosition);
		MapChipField::IndexSet rightIndexSet = mapChipField_->GetMapChipIndexByPosition(rightBottomPosition);

		MapChipType leftMapChipType = mapChipField_->GetMapChipTypeIndex(leftIndexSet.xIndex, leftIndexSet.yIndex);
		MapChipType rightMapChipType = mapChipField_->GetMapChipTypeIndex(rightIndexSet.xIndex, rightIndexSet.yIndex);

		if (leftMapChipType == MapChipType::kBlock || rightMapChipType == MapChipType::kBlock) {
			hit = true;
		}
		if (!hit) {
			onGround_ = false;
		}
	} else {
		const float footY = CornerPosition(worldTransform_.translation_, Corner::kLeftBottom).y;
		if (info.isFloorCollision && worldTransform_.translation_.y <= footY) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
		}
	}
}

void IsMapCollision(CollisionMapInfo& info, const WorldTransform& worldTransform, MapChipField* mapChipField) {
	if (!mapChipField)
		return;
	IsTopCollision(info, worldTransform, mapChipField);
	IsBottomCollision(info, worldTransform, mapChipField);
	IsRightCollision(info, worldTransform, mapChipField);
	IsLeftCollision(info, worldTransform, mapChipField);
}

void IsTopCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	if (!mapChipField)
		return;

	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.y <= 0.0f)
		return;

	bool hit = false;
	MapChipField::IndexSet indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	MapChipType mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
		hit = true;

	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
		hit = true;

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float newY = static_cast<float>(rect.bottom) - 3.0f - kHeight;
		if (newY < 0.0f)
			newY = 0.0f;
		info.movement.y = newY;
		info.isCeilingCollision = true;
	}
}

void IsBottomCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	if (!mapChipField)
		return;

	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.y >= 0.0f)
		return;

	bool hit = false;
	MapChipField::IndexSet indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	MapChipType mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
		hit = true;

	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock)
		hit = true;

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex - 1);
		float newY = static_cast<float>(rect.top) + kHeight + 1.0f;
		if (newY > 0.0f)
			newY = 0.0f;
		info.movement.y = newY;
		info.isFloorCollision = true;
	}
}

void IsRightCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	if (!mapChipField)
		return;

	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.x <= 0.0f)
		return;

	bool hit = false;
	MapChipField::IndexSet indexSet;

	positionsNew[kRightTop].x -= 0.5f;
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightTop]);
	if (mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	positionsNew[kRightBottom].x -= 0.5f;
	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
	if (mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float newX = static_cast<float>(rect.left) - 1.0f;
		if (newX > 0.0f)
			newX = 0.0f;
		info.movement.x = newX;
		info.isWallCollision = true;
	}
}

void IsLeftCollision(CollisionMapInfo& info, const WorldTransform& worldTransform_, MapChipField* mapChipField) {
	if (!mapChipField)
		return;

	std::array<Vector3, 4> positionsNew;
	for (uint32_t i = 0; i < 4; ++i) {
		positionsNew[i] = CornerPosition(Add(worldTransform_.translation_, info.movement), static_cast<Corner>(i));
	}

	if (info.movement.x >= 0.0f)
		return;

	positionsNew[kLeftTop].x -= 0.6f;
	positionsNew[kLeftBottom].x -= 0.6f;

	bool hit = false;
	MapChipField::IndexSet indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftTop]);
	if (mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
	if (mapChipField->GetMapChipTypeIndex(indexSet.xIndex, indexSet.yIndex) == MapChipType::kBlock)
		hit = true;

	if (hit) {
		indexSet = mapChipField->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
		MapChipField::Rect rect = mapChipField->GetMapChipRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float newX = static_cast<float>(rect.right) + 1.0f;
		if (newX > 0.0f)
			newX = 0.0f;
		info.movement.x = newX;
		info.isWallCollision = true;
	}
}

Vector3 CornerPosition(const Vector3& centor, Corner corner) {
	Vector3 offsetTable[kNumCorner]{
	    {kWidth / 2.0f,  -kHeight / 2.0f, 0.0f},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
        {kWidth / 2.0f,  kHeight / 2.0f,  0.0f},
        {-kWidth / 2.0f, kHeight / 2.0f,  0.0f}
    };

	// 元実装の意図に合わせて x,y を /2 する
	return {
	    (centor.x + offsetTable[static_cast<uint32_t>(corner)].x) / 2.0f, (centor.y + offsetTable[static_cast<uint32_t>(corner)].y) / 2.0f, centor.z + offsetTable[static_cast<uint32_t>(corner)].z};
}
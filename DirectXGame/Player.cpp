#define NOMINMAX
#include "Player.h"
#include "MapChipFiled.h"
#include "WorldTransformClass.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>
#include <array> 

KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Player::Corner corner) {
	KamataEngine::Vector3 offsetTable[Player::Corner::kNumCenter] = {
	    {+Player::kWidth / 2.0f, -Player::kHeight / 2.0f, 0.0f}, // kRightBottom
	    {-Player::kWidth / 2.0f, -Player::kHeight / 2.0f, 0.0f}, // kLeftBottom
	    {+Player::kWidth / 2.0f, +Player::kHeight / 2.0f, 0.0f}, // kRightTop
	    {-Player::kWidth / 2.0f, +Player::kHeight / 2.0f, 0.0f}, // kLeftTop
	};

	KamataEngine::Vector3 offset = offsetTable[static_cast<uint32_t>(corner)];
	return {center.x + offset.x, center.y + offset.y, center.z + offset.z};
}

// そのタイルが「当たりあり」か？
inline bool IsSolid(const MapChipField* field, uint32_t xi, uint32_t yi) { return field->GetMapChipTypeByIndex(xi, yi) == MapChipField::MapChipType::kBlock; }

// ワールド座標が刺さっているタイルが「当たりあり」か？
inline bool IsSolidAt(const MapChipField* field, const KamataEngine::Vector3& pos) {
	auto idx = field->GetMapChipIndexByPosition(pos);
	return IsSolid(field, idx.xIndex, idx.yIndex);
}

void Player::MapCllisionCheckUp(Player::ColisionMapInfo& info) {
	if (info.velosity_.y <= 0.0f)
		return; // 上昇時のみ

	const float topY_now = worldTransform_.translation_.y + kHeight * 0.5f;
	const float nextTopY = topY_now + info.velosity_.y;
	const float probeY = nextTopY + smallNum;

	std::array<KamataEngine::Vector3, 3> probe = {
	    {
         {worldTransform_.translation_.x - kWidth * 0.5f + smallNum, probeY, worldTransform_.translation_.z},
         {worldTransform_.translation_.x, probeY, worldTransform_.translation_.z},
         {worldTransform_.translation_.x + kWidth * 0.5f - smallNum, probeY, worldTransform_.translation_.z},
	     }
    };

	bool hit = false;
	float minCeilingTop = std::numeric_limits<float>::infinity(); // ←「最も低い」天井の top を記録

	for (const auto& p : probe) {
		if (IsSolidAt(mapChipField_, p)) {
			auto idx = mapChipField_->GetMapChipIndexByPosition(p);
			auto rect = mapChipField_->GetRectByIndex(idx.xIndex, idx.yIndex);

			// ↑↑ 上方向の衝突面は rect.top（下向きの面）
			minCeilingTop = std::min(minCeilingTop, rect.top);
			hit = true;
		}
	}

	if (hit) {
		// プレイヤーの top を rect.top の少し手前にクランプ
		float allowedMove = (minCeilingTop - smallNum) - topY_now;
		if (allowedMove < info.velosity_.y) {
			info.velosity_.y = allowedMove;
		}
		info.celling = true;
	}
}




void Player::MapCllisionCheckDown(Player::ColisionMapInfo& info) {
	if (info.velosity_.y >= 0.0f)
		return; // 下向き移動のときだけ

	const float bottomY_now = worldTransform_.translation_.y - kHeight * 0.5f;
	const float nextBottomY = bottomY_now + info.velosity_.y;

	// 左・中央・右の3点で調べる（角抜け軽減）
	const std::array<float, 3> probeXs = {
	    {worldTransform_.translation_.x - kWidth * 0.5f + smallNum, worldTransform_.translation_.x, worldTransform_.translation_.x + kWidth * 0.5f - smallNum}
    };

	bool hit = false;
	float highestTop = -std::numeric_limits<float>::infinity(); // 区間中で一番“高い”床

	for (float px : probeXs) {
		// いま居る行と、次に着地しようとしている行を取得
		// （少し内側をサンプリングしてインデックス境界の誤差を避ける）
		auto idxNow = mapChipField_->GetMapChipIndexByPosition({px, bottomY_now - smallNum, worldTransform_.translation_.z});
		auto idxNext = mapChipField_->GetMapChipIndexByPosition({px, nextBottomY - smallNum, worldTransform_.translation_.z});

		// 下に動くと yIndex は増える（上→下）
		uint32_t yiStart = std::min(idxNow.yIndex, idxNext.yIndex);
		uint32_t yiEnd = std::max(idxNow.yIndex, idxNext.yIndex);

		// 通過する各行で、該当タイルがブロックなら床候補にする
		for (uint32_t yi = yiStart; yi <= yiEnd; ++yi) {
			auto type = mapChipField_->GetMapChipTypeByIndex(idxNow.xIndex, yi);
			if (type != MapChipField::MapChipType::kBlock)
				continue;

			auto rect = mapChipField_->GetRectByIndex(idxNow.xIndex, yi);
			highestTop = std::max(highestTop, rect.top + smallNum); // 少し上に余白
			hit = true;

			// 一番手前の床だけで十分なら break してもOK
			// if (rect.top + smallNum >= bottomY_now) break;
		}
	}

	if (hit) {
		// プレイヤーの bottom を highestTop に揃えるように移動量をクランプ
		const float desiredCenterY = highestTop + kHeight * 0.5f;
		const float allowedMove = desiredCenterY - worldTransform_.translation_.y; // 負〜ゼロ
		// 「より上向き（0に近い）」補正を選ぶ
		info.velosity_.y = std::max(info.velosity_.y, allowedMove);
		info.landing = true;
	}
}



void Player::MapCllisionCheckRight(Player::ColisionMapInfo& info) {
	if (info.velosity_.x <= 0.0f)
		return;

	const float nextRightX = (worldTransform_.translation_.x + info.velosity_.x) + kWidth * 0.5f;

	// プレイヤの縦区間（角を少し避ける）
	constexpr float cornerSlop = 0.10f;
	const float playerTop = worldTransform_.translation_.y + (kHeight * 0.5f - cornerSlop);
	const float playerBottom = worldTransform_.translation_.y - (kHeight * 0.5f - cornerSlop);

	std::array<KamataEngine::Vector3, 3> probe = {
	    {
         {nextRightX, playerBottom, worldTransform_.translation_.z},
         {nextRightX, worldTransform_.translation_.y, worldTransform_.translation_.z},
         {nextRightX, playerTop, worldTransform_.translation_.z},
	     }
    };

	bool hit = false;
	float bestV = info.velosity_.x; // 正（右向き）

	for (const auto& p : probe) {
		if (!IsSolidAt(mapChipField_, p))
			continue;

		auto idx = mapChipField_->GetMapChipIndexByPosition(p);
		auto rect = mapChipField_->GetRectByIndex(idx.xIndex, idx.yIndex);

		// 角の点接触を除外：縦方向にしっかり重なっているかを見る
		const bool verticalOverlap = (playerBottom < rect.bottom - cornerSlop) && (playerTop > rect.top + cornerSlop);
		if (!verticalOverlap)
			continue;

		// はみ出し量（負）。相対クランプ：予定の移動量に「戻し量」を足す
		const float overlap = rect.left - nextRightX;                  // <= 0
		const float candidate = info.velosity_.x + overlap + kEpsilon; // <= info.velosity_.x

		// 右向き移動の“上限”に絞る。逆向き（負）にはしない
		bestV = std::min(bestV, std::max(0.0f, candidate));
		hit = true;
	}

	if (hit) {
		info.velosity_.x = bestV;
		info.isWall = true;
	}
}


void Player::MapCllisionCheckLeft(Player::ColisionMapInfo& info) {
	if (info.velosity_.x >= 0.0f)
		return;

	const float nextLeftX = (worldTransform_.translation_.x + info.velosity_.x) - kWidth * 0.5f;

	constexpr float cornerSlop = 0.10f;
	const float playerTop = worldTransform_.translation_.y + (kHeight * 0.5f - cornerSlop);
	const float playerBottom = worldTransform_.translation_.y - (kHeight * 0.5f - cornerSlop);

	std::array<KamataEngine::Vector3, 3> probe = {
	    {
         {nextLeftX, playerBottom, worldTransform_.translation_.z},
         {nextLeftX, worldTransform_.translation_.y, worldTransform_.translation_.z},
         {nextLeftX, playerTop, worldTransform_.translation_.z},
	     }
    };

	bool hit = false;
	float bestV = info.velosity_.x; // 負（左向き）

	for (const auto& p : probe) {
		if (!IsSolidAt(mapChipField_, p))
			continue;

		auto idx = mapChipField_->GetMapChipIndexByPosition(p);
		auto rect = mapChipField_->GetRectByIndex(idx.xIndex, idx.yIndex);

		const bool verticalOverlap = (playerBottom < rect.bottom - cornerSlop) && (playerTop > rect.top + cornerSlop);
		if (!verticalOverlap)
			continue;

		const float overlap = rect.right - nextLeftX;                  // >= 0
		const float candidate = info.velosity_.x + overlap - kEpsilon; // >= info.velosity_.x

		// 左向き移動の“下限”に絞る。逆向き（正）にはしない
		bestV = std::max(bestV, std::min(0.0f, candidate));
		hit = true;
	}

	if (hit) {
		info.velosity_.x = bestV;
		info.isWall = true;
	}
}


void Player::OnSwichGround(const Player::ColisionMapInfo& info) {
	// 自キャラが接地状態？
	if (onGround_) {

		// ジャンプ開始
		if (velosity_.y > 0.0f) {
			onGround_ = false;
		}

		std::array<Vector3, Player::Corner::kNumCenter> positionsNew;
		Vector3 worldPos = {worldTransform_.translation_.x + info.velosity_.x, worldTransform_.translation_.y + info.velosity_.y, worldTransform_.translation_.z + info.velosity_.z};

		for (uint32_t i = 0; i < positionsNew.size(); ++i) {
			positionsNew[i] = CornerPosition(worldPos, static_cast<Player::Corner>(i));
		}
		MapChipField::MapChipType mapChipType;
		// 真上のあたり判定を行う
		bool hit = false;
		// 左下の判定
		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kLeftBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipField::MapChipType::kBlock) {
			hit = true;
		}
		// 右下の判定
		indexSet = mapChipField_->GetMapChipIndexByPosition(positionsNew[kRightBottom]);
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipField::MapChipType::kBlock) {
			hit = true;
		}

		// 落下開始
		if (!hit) {
			// 空中状態に切り替える
			onGround_ = false;
		}

	} else {

		// 着地フラグ
		if (info.landing) {
			// 着地状態に切り替える(落下を止める)
			onGround_ = true;
			// 着地時にX座標を減衰
			velosity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velosity_.y = 0.0f;
		}
	}
}

void Player::IsWall(Player::ColisionMapInfo& info) {
	if (info.isWall) {
		info.velosity_.x *= (1.0f - kAttenuationWall);
	}
}

void MoveAccordingResult(WorldTransform& worldTransform, const Player::ColisionMapInfo& info) {

	worldTransform.translation_.x += info.velosity_.x;
	worldTransform.translation_.y += info.velosity_.y;
	worldTransform.translation_.z += info.velosity_.z;
}

void Player::IsCelling(Player::ColisionMapInfo& info) {

	if (info.celling) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		info.velosity_.y = 0;
	}
}

void Player::MapCollisionCheck(ColisionMapInfo& info) {
	MapCllisionCheckUp(info);
	MapCllisionCheckDown(info);
	MapCllisionCheckRight(info);
	MapCllisionCheckLeft(info);
}

KamataEngine::Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {

	AABB aabb;
	aabb.min = {worldTransform_.translation_.x - kWidth / 2.0f, worldTransform_.translation_.y - kHeight / 2.0f, worldTransform_.translation_.z - kWidth / 2.0f};
	aabb.max = {worldTransform_.translation_.x + kWidth / 2.0f, worldTransform_.translation_.y + kHeight / 2.0f, worldTransform_.translation_.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// velosity_.x += 1.0f;
	// velosity_.y += 1.0f;
	// velosity_.z += 1.0f;

	if (IsAttack()) {
		return; // 無敵中なら何もしない
	}

	isDead_ = true;
}

void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Model* attackModel, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {

	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	camera_ = camera;
	attackModel_ = attackModel;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	worldTransformAttack_.Initialize();
	worldTransformAttack_.translation_ = position;
	worldTransformAttack_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformAttack_.rotation_.y = std::numbers::pi_v<float> / 2.0f; // 初期角度を90度に設定
}

// 目的地（dx,dy: タイル単位）が空いていればステップ開始
bool Player::TryStartStep(int dx, int dy) {
	if (isStepping_ || !mapChipField_)
		return false;

	const float tileW = MapChipField::kBlockWidth;
	const float tileH = MapChipField::kBlockHeight;

	// 目標中心座標 = 現在位置 + タイル分
	KamataEngine::Vector3 goal = worldTransform_.translation_;
	goal.x += dx * tileW;
	goal.y += dy * tileH;

	// 目標マスがブロックなら進めない
	auto idx = mapChipField_->GetMapChipIndexByPosition(goal);
	if (mapChipField_->GetMapChipTypeByIndex(idx.xIndex, idx.yIndex) == MapChipField::MapChipType::kBlock) {
		return false;
	}

	// ステップ開始
	isStepping_ = true;
	stepFrame_ = 0;
	stepStart_ = worldTransform_.translation_;
	stepGoal_ = goal;

	// 向き更新（任意：左右だけ）
	if (dx > 0) {
		lrDirection_ = LRDirection::kRight;
		turnTimer_ = 1.0f;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
	}
	if (dx < 0) {
		lrDirection_ = LRDirection::kLeft;
		turnTimer_ = 1.0f;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
	}

	return true;
}

// ステップ補間（毎フレーム呼ぶ）
void Player::UpdateStep() {
	if (!isStepping_)
		return;

	float t = static_cast<float>(stepFrame_) / static_cast<float>(kStepFrames);
	t = std::clamp(t, 0.0f, 1.0f);

	// 線形補間（必要なら EaseOut/EaseIn に差し替え可）
	worldTransform_.translation_.x = stepStart_.x + (stepGoal_.x - stepStart_.x) * t;
	worldTransform_.translation_.y = stepStart_.y + (stepGoal_.y - stepStart_.y) * t;

	if (++stepFrame_ >= kStepFrames) {
		worldTransform_.translation_ = stepGoal_;
		isStepping_ = false;
		stepFrame_ = 0;
	}
}


void Player::PlayerMove() {
	if (isStepping_)
		return; // ステップ中は新入力を受け付けない

	auto* in = KamataEngine::Input::GetInstance();
	int dx = 0, dy = 0;

	if (in->TriggerKey(DIK_RIGHT))
		dx = +1;
	else if (in->TriggerKey(DIK_LEFT))
		dx = -1;
	else if (in->TriggerKey(DIK_UP))
		dy = +1; // 上へ1マス（座標系が上＋の場合）
	else if (in->TriggerKey(DIK_DOWN))
		dy = -1; // 下へ1マス

	if (dx != 0 || dy != 0) {
		TryStartStep(dx, dy);
	}
}



void Player::TurnUpdata() {
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 30.0f;
		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,

		};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 補間率
		float t = 1.0f - turnTimer_;
		t = std::clamp(t, 0.0f, 1.0f); // 念のため

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = worldTransform_.rotation_.y = Lerp(turnFirstRotationY_, destinationRotationY, t);
		;
	}
}

void Player::WorldPosUpdate(const ColisionMapInfo& info) {
	// 移動量を反映させる
	worldTransform_.translation_.x += info.velosity_.x;
	worldTransform_.translation_.y += info.velosity_.y;
	worldTransform_.translation_.z += info.velosity_.z;
}

void Player::BehaviorRootInitialize() { velosity_ = {0.0f, 0.0f, 0.0f}; }

void Player::BehaviorAttackInitialize() {
	// カウンター初期化
	attackParameter_ = 0;
	// 予備動作
	attackParameter_++;

	velosity_ = {0.0f, 0.0f, 0.0f};

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// 攻撃フェーズをチャージ状態に設定
	attakFase_ = AttakFase::kChage;

	if (lrDirection_ == LRDirection::kRight) {

		correctionTransform.translation_.x = 2.0f;

	} else if (lrDirection_ == LRDirection::kLeft) {

		correctionTransform.translation_.x = -2.0f;
	}

	isAttack_ = true; // 攻撃開始時に無敵にする
}

void Player::ChageUpdate() {

	switch (attakFase_) {

	case Player::AttakFase::kChage:
	default: {

		float t = static_cast<float>(attackParameter_) / chageTime;
		worldTransform_.scale_.z = EaseOut(1.0f, 0.3f, t);
		worldTransform_.scale_.y = EaseOut(1.0f, 2.0f, t);
		// 前進動作へ移行
		if (attackParameter_ >= chageTime) {
			attakFase_ = Player::AttakFase::kAttack;
			attackParameter_ = 0; // カウンター初期化
		}

		break;
	}
	}
}

void Player::AttackUpdate() {

	switch (attakFase_) {

	case Player::AttakFase::kAttack:

		float t = static_cast<float>(attackParameter_) / kAttackTime;
		worldTransform_.scale_.z = EaseOut(0.3f, 1.3f, t);
		worldTransform_.scale_.y = EaseIn(1.6f, 0.7f, t);
		// 攻撃動作終了
		if (attackParameter_ >= kAttackTime) {
			attakFase_ = Player::AttakFase::kAfterglow;
			attackParameter_ = 0; // カウンター初期化
		}

		break;
	}
}

void Player::AfterglowUpdate() {
	switch (attakFase_) {
	case Player::AttakFase::kAfterglow:

		// 余韻動作のスケール変化
		float t = static_cast<float>(attackParameter_) / kAfterglowTime;
		worldTransform_.scale_.z = EaseOut(1.3f, 1.0f, t);
		worldTransform_.scale_.y = EaseOut(0.7f, 1.0f, t);

		break;
	}
}

void Player::BehaviorRootUpdate() {

	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 攻撃ビヘイビアをリクエスト
		behaviorRequest_ = Behavior::kAttak;
	}
}

void Player::BehaviorAttackUpdate() {

	attackParameter_++;

	Vector3 attackVelosity = {}; // 初期化

	// 攻撃フェーズごとの更新処理
	switch (attakFase_) {
	case Player::AttakFase::kChage:
		ChageUpdate();
		break;

	case Player::AttakFase::kAttack:
		AttackUpdate();

		// 攻撃中はX方向に突進
		if (lrDirection_ == LRDirection::kRight) {
			attackVelosity.x = attakVelosity;
			correctionTransform.translation_.x = 2.0f;

		} else if (lrDirection_ == LRDirection::kLeft) {
			attackVelosity.x = -attakVelosity;
			correctionTransform.translation_.x = -2.0f;
		}

		break;

	case Player::AttakFase::kAfterglow:
		AfterglowUpdate();

		// X方向は止まる（余韻）、Y方向には落下する
		attackVelosity.x = 0.0f;
		attackVelosity.y = velosity_.y - kGravityAcceleration;
		attackVelosity.y = std::max(attackVelosity.y, -kLimitFallSpeed);
		break;
	}
	velosity_ = attackVelosity;

	// フェーズがすべて終わったら元に戻す
	if (attackParameter_ > kAttackTime + kAfterglowTime) {
		behaviorRequest_ = Behavior::kRoot;
		velosity_.x = 0.0f;
		attackParameter_ = 0;
		attakFase_ = AttakFase::kUnKnown;

		isAttack_ = false;
	}

	// トランスフォームの値のコピー
	worldTransformAttack_.translation_ = worldTransform_.translation_;
	// worldTransformAttack_.translation_.x += correctionTransform.translation_.x;
	worldTransformAttack_.rotation_ = worldTransform_.rotation_;
}

void Player::Update() {

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	worldTransformAttack_.matWorld_ = MakeAffineMatrix(worldTransformAttack_.scale_, worldTransformAttack_.rotation_, worldTransformAttack_.translation_);
	worldTransformAttack_.TransferMatrix();

	ColisionMapInfo collisionMapInfo;

	// 移動処理
	if (behavior_ == Behavior::kRoot) {
		PlayerMove(); // 通常時のみ移動
	}
	TurnUpdata();

	/*if (worldTransform_.translation_.y < 2.0f) {
	    worldTransform_.translation_.y = 2.0f;

	}*/

	if (behaviorRequest_ != Behavior::kUnKnown) {

		// 振る舞いを変更する
		behavior_ = behaviorRequest_;

		// 各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Player::Behavior::kRoot:

			BehaviorRootInitialize();

			break;
		case Player::Behavior::kAttak:

			BehaviorAttackInitialize();

			break;
		}
		// 振る舞いのリクエストをクリア
		behaviorRequest_ = Behavior::kUnKnown;
	}

	switch (behavior_) {
	case Player::Behavior::kRoot:

		BehaviorRootUpdate();

		break;
	case Player::Behavior::kAttak:

		BehaviorAttackUpdate();

		break;
	}

	collisionMapInfo.velosity_ = velosity_;

	//if (velosity_.y == 0) {
	//	onGround_ = true;
	//	collisionMapInfo.landing = true;
	//}

	// --- Y axis first ---
	Player::ColisionMapInfo yInfo{};
	yInfo.velosity_ = {0.0f, velosity_.y, 0.0f};
	MapCllisionCheckUp(yInfo);
	MapCllisionCheckDown(yInfo);

	// Y座標を反映
	worldTransform_.translation_.y += yInfo.velosity_.y;

	// 地上/天井の状態更新（ここで onGround_ や velosity_.y を0にする）
	OnSwichGround(yInfo);

	// Y速度はヒット時0固定、非ヒット時のみ反映
	// Updata のY速度確定部分を修正
	if (yInfo.landing) {
		velosity_.y = 0.0f; // 着地なら落下停止
	} else if (yInfo.celling) {
		velosity_.y = 0.0f; // 天井なら上昇停止
	} else {
		velosity_.y = yInfo.velosity_.y; // それ以外はそのまま
	}

	// --- X axis second ---
	Player::ColisionMapInfo xInfo{};
	xInfo.velosity_ = {velosity_.x, 0.0f, 0.0f};
	MapCllisionCheckRight(xInfo);
	MapCllisionCheckLeft(xInfo);

	// X座標を反映
	worldTransform_.translation_.x += xInfo.velosity_.x;

	// 壁ヒット時は速度0、非ヒット時のみ反映
	velosity_.x = xInfo.isWall ? 0.0f : xInfo.velosity_.x;

	// マップ衝突判定
	MapCollisionCheck(collisionMapInfo);
	WorldPosUpdate(collisionMapInfo);
	OnSwichGround(collisionMapInfo);
	IsCelling(yInfo);
	IsWall(xInfo);
	// 判定結果を反映させて移動させる

	 if (behavior_ == Behavior::kRoot) {
		PlayerMove();
	}

	// ステップ中なら補間だけ進めて終了（速度処理や連続衝突は使わない）
	if (isStepping_) {
		UpdateStep();

		// ここで行列転送や描画用更新は従来通り実施
		worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
		worldTransform_.TransferMatrix();
		WorldTrnasformUpdate(worldTransform_);
		// 攻撃モデル同期など必要ならここで…

		return; // ← 連続衝突や速度更新には入らない
	}

	if (behavior_ == Behavior::kAttak) {
		worldTransformAttack_.translation_ = worldTransform_.translation_;

		// 進行方向へ少し前に出したい場合はここで補正
		//			worldTransformAttack_.translation_.x += correctionTransform.translation_.x;

		worldTransformAttack_.rotation_ = worldTransform_.rotation_;
	}

	WorldTrnasformUpdate(worldTransformAttack_);
	WorldTrnasformUpdate(worldTransform_);

	ImGui::Text("isWall  : %s", collisionMapInfo.isWall ? "true" : "false");
	ImGui::Text("isGround  : %s", collisionMapInfo.landing ? "true" : "false");
	ImGui::Text("onGround  : %s", onGround_ ? "true" : "false");
	ImGui::Text("playerY  : %f", worldTransform_.translation_.y);
	ImGui::Text("velsity.y  : %f", velosity_.y);
	ImGui::Text("velsity.x  : %f", velosity_.x);
	ImGui::Text("Player rotationY: %f", worldTransform_.rotation_.y);
	ImGui::Text("AttackModel rotationY: %f", worldTransformAttack_.rotation_.y);
	ImGui::Text("AttakModel worldTransform translation: %f, %f, %f", worldTransformAttack_.translation_.x, worldTransformAttack_.translation_.y, worldTransformAttack_.translation_.z);
	
	// こうする:
	ImGui::Text("yInfo.landing : %s", yInfo.landing ? "true" : "false");
	ImGui::Text("yInfo.celling : %s", yInfo.celling ? "true" : "false");
	ImGui::Text("xInfo.isWall  : %s", xInfo.isWall ? "true" : "false");
	ImGui::Text("yMove Applied : %.4f", yInfo.velosity_.y);
	ImGui::Text("xMove Applied : %.4f", xInfo.velosity_.x);
}

void Player::TitleUpdata() {
	// タイトルシーンの更新処理
	// タイマーを増加（回転速度）
	rotateTimer_ += 1.0f;

	// Y軸に回転を適用
	worldTransform_.rotation_.y = rotateTimer_;
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	WorldTrnasformUpdate(worldTransform_);
}

void Player::Draw() {

	// 自キャラの描画
	model_->Draw(worldTransform_, *camera_);
	// 攻撃モデルの描画

	
	if (attakFase_ == AttakFase::kAttack) {
		attackModel_->Draw(worldTransformAttack_, *camera_);
	}
}

void Player::SetPosition(const KamataEngine::Vector3& position) { worldTransform_.translation_ = position; }

const KamataEngine::WorldTransform& Player::GetWoldTransform() const { return worldTransform_; }

// EnemyState_Approach.cpp
#include "EnemyState_Approach.h"
#include "Enemy.h"
#include "EnemyState_Leave.h" // 次の状態に遷移するため

void EnemyState_Approach::Enter(Enemy* enemy) {
	enemy->ApproachInitialize(); // ← 初期化処理を呼び出す
}


void EnemyState_Approach::Update(Enemy* enemy) {
	const float kMoveSpeed = 0.1f;
	KamataEngine::Vector3 velocity = {0, 0, -kMoveSpeed};
	enemy->Move(velocity);

	// ※ここでの FireTimer の手動カウント・Fire 呼び出しは不要

	// 状態遷移チェック
	if (enemy->GetPosition().z < 0.0f) {
		enemy->SetState(new EnemyState_Leave());
	}
}

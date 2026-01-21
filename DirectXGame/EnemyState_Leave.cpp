// EnemyState_Leave.cpp
#include "EnemyState_Leave.h"
#include "Enemy.h"

void EnemyState_Leave::Update(Enemy* enemy) {
	const float kMoveSpeed = 0.05f;
	KamataEngine::Vector3 velocity = {-kMoveSpeed, kMoveSpeed, -kMoveSpeed};

	enemy->Move(velocity);
}

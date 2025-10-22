// EnemyState_Leave.h
#pragma once
#include "EnemyState.h"

class EnemyState_Leave : public EnemyState {
public:
	void Update(Enemy* enemy) override;
};

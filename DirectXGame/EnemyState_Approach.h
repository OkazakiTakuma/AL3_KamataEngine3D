// EnemyState_Approach.h
#pragma once
#include "EnemyState.h"

class EnemyState_Approach : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
};

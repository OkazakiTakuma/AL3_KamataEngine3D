#pragma once

class Enemy; // 前方宣言

class EnemyState {
public:
	virtual ~EnemyState() = default;

	virtual void Enter(Enemy* /*enemy*/) {} // ← 追加：初期化用（デフォルト空）

	// 状態ごとの処理
	virtual void Update(Enemy* enemy) = 0;
};

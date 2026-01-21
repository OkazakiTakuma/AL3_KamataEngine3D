#pragma once
#include "Collider.h"
#include <list>

class CollisionManager {
public:
	// コライダーの登録
	void AddCollider(Collider* collider) { colliders_.push_back(collider); }

	// 衝突判定
	void CheckAllCollisions();

	// コライダーリストをクリア
	void Clear() { colliders_.clear(); }

private:
	std::list<Collider*> colliders_;
};
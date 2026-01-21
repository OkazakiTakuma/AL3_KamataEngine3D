// CollisionManager.cpp
#include "CollisionManager.h"
#include "KamataEngine.h" // 必要に応じて

using namespace KamataEngine;

void CollisionManager::CheckAllCollisions() {
	for (auto itrA = colliders_.begin(); itrA != colliders_.end(); ++itrA) {
		auto itrB = itrA;
		++itrB;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* a = *itrA;
			Collider* b = *itrB;

			// フィルタリング
			if ((a->GetCollisionMask() & b->GetCollisionAttribute()) == 0 || (b->GetCollisionMask() & a->GetCollisionAttribute()) == 0) {
				continue;
			}

			// 当たり判定
			Vector3 posA = a->GetWorldPosition();
			Vector3 posB = b->GetWorldPosition();
			float dx = posA.x - posB.x;
			float dy = posA.y - posB.y;
			float dz = posA.z - posB.z;
			float distSq = dx * dx + dy * dy + dz * dz;

			float rSum = a->GetRadius() + b->GetRadius();
			if (distSq <= rSum * rSum) {
				a->OnCollision();
				b->OnCollision();
			}
		}
	}
}

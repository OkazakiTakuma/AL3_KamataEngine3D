#pragma once
#include "KamataEngine.h"

class AABB {
public:
	KamataEngine::Vector3 min; // 最小座標
	KamataEngine::Vector3 max; // 最大座標

	bool IsCollisionAABB(const AABB& aabb1, const AABB& aabb2);

};

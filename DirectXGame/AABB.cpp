#include "AABB.h"

bool AABB::IsCollisionAABB(const AABB& aabb1, const AABB& aabb2) {
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // X
	       (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // Y
	       (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);   // Z
}

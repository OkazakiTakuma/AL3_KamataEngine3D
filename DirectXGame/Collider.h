
#pragma once
#include "KamataEngine.h"
using KamataEngine::Vector3;

class Collider {

public:
	float GetRadius() const { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }
	virtual Vector3 GetWorldPosition() const = 0; // 純粋仮想関数
	virtual void OnCollision() = 0;               // 純粋仮想関数

	  void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	void SetCollisionMask(uint32_t mask) { collisionMask_ = mask; }

	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
	uint32_t GetCollisionMask() const { return collisionMask_; }

private:
	float radius_ = 1.0f; // デフォルト半径

	uint32_t collisionAttribute_ = 0xffffffff; // 自分の属性（例：プレイヤー）
	uint32_t collisionMask_ = 0xffffffff;      // 何に当たるか（マスク）
};
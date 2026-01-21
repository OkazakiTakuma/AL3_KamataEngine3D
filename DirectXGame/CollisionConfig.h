#pragma once
#include <cstdint>

// 属性のビット定義
const uint32_t kCollisionAttributePlayer = 0b1 << 0; // プレイヤー
const uint32_t kCollisionAttributeEnemy = 0b1 << 1;  // 敵
const uint32_t kCollisionAttributePlayerBullet = 0b1 << 2;
const uint32_t kCollisionAttributeEnemyBullet = 0b1 << 3;
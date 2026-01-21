#pragma once
#include "KamataEngine.h" // Vector3 を含むヘッダー

namespace KamataEngine {

// 加算（+=）
inline Vector3& operator+=(Vector3& lhs, const Vector3& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

// 減算（-=）
inline Vector3& operator-=(Vector3& lhs, const Vector3& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}

// スカラー乗算（*=）
inline Vector3& operator*=(Vector3& lhs, float scalar) {
	lhs.x *= scalar;
	lhs.y *= scalar;
	lhs.z *= scalar;
	return lhs;
}

// 加算（+）
inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs) { return Vector3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z}; }

// 減算（-）
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs) { return Vector3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z}; }

// スカラー乗算（*）
inline Vector3 operator*(const Vector3& vec, float scalar) { return Vector3{vec.x * scalar, vec.y * scalar, vec.z * scalar}; }
inline Vector3 operator*(float scalar, const Vector3& vec) {
	return vec * scalar; // 上と同じ順序で呼び出し
}

} // namespace KamataEngine

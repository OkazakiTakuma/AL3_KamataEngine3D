#pragma once
#define NOMINMAX
#include "algorithm"
// 線形補間
inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }

// EaseOut（2乗曲線）
inline float EaseOut(float x, float y, float t) {
	t = std::clamp(t, 0.0f, 1.0f);
	float easedT = 1.0f - (1.0f - t) * (1.0f - t);
	return x + (y - x) * easedT;
}
// EaseIn（2乗曲線）
inline float EaseIn(float x, float y, float t) {
	t = std::clamp(t, 0.0f, 1.0f);
	float easedT = t * t;
	return x + (y - x) * easedT;
}
#pragma once
#include "Matrix.h"
#include <vector>
#include <cmath>
#include"KamataEngine.h"
#include<algorithm>




/// <summary>
/// ワールドトランスフォームの計算と転送
/// </summary>
void WorldTransformUpdate(KamataEngine::WorldTransform& worldTransform);

KamataEngine::Vector3 Leap(KamataEngine::Vector3 startPosition, KamataEngine::Vector3 endPosition, float speed);
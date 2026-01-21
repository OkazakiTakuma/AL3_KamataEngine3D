#pragma once
#define NOMINMAX
#include "KamataEngine.h"
#include <Windows.h>
#include <vector>
#include "algorithm"

void WorldTrnasformUpdate(KamataEngine::WorldTransform& worldTransform);

KamataEngine::Matrix4x4 MakeAffineMatrix(const KamataEngine::Vector3& scale, 
	const KamataEngine::Vector3& rotate, 
	const KamataEngine::Vector3& translate);



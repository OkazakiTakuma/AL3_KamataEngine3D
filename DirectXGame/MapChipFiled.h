#pragma once
#include "KamataEngine.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace KamataEngine;

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	// マップチップの種類
	enum class MapChipType {
		kBlank, // 空白
		kBlock  // ブロック
	};

	struct Rect {

		float left;   // 左端
		float right;  // 右端
		float bottom; // 下端
		float top;    // 上端
	};

	// マップチップのデータ構造体
	struct MapChipData {
		std::vector<std::vector<MapChipType>> data;
	};

	struct IndexSet {

		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 定数（ブロックサイズと個数）
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	// マップチップデータのリセット
	void ResetMapChipData();

	// マップチップCSVの読み込み
	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) const;

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	// ブロック数取得
	uint32_t GetNumBlockVirtical() { return kNumBlockVirtical; }
	uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }

	IndexSet GetMapChipIndexByPosition(const KamataEngine::Vector3& position)const;
	
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	// マップチップのデータ
	MapChipData mapChipData_;
};
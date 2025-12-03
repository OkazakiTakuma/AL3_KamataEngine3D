#pragma once
#include "KamataEngine.h"
#include "Matrix.h"



enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
	kLadder, // はしご
	kCrackBlock, // 崩れる床
};
struct MapchipData {
	std::vector<std::vector<MapChipType>> data;
};
class MapChipField {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
	
		float left;
		float right;
		float bottom;
		float top;
	};

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	IndexSet GetMapChipIndexByPosition(const KamataEngine::Vector3& position);
	Rect GetMapChipRectByIndex(uint32_t xIndex, uint32_t yIndex);
	void SetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex, MapChipType type);

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	// mapChipData_ への public アクセス用 getter を追加

private:
	MapchipData mapChipData_;
	IndexSet mapChipIndex_;
};

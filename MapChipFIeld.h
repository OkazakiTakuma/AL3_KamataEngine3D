#pragma once
#include "KamataEngine.h"
#include "Matrix.h"

enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};
struct MapchipData {
	std::vector<std::vector<MapChipType>> data;
};
class MapChipFIeld {
public:
	
	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
		

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
	
	MapchipData mapChipData_;

	
};

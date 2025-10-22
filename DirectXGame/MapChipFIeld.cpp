#include "MapChipFiled.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
// #include "Matrix4x4.h"

namespace {
std::map<std::string, MapChipField::MapChipType> mapChipTable = {
    {"0", MapChipField::MapChipType::kBlank},
    {"1", MapChipField::MapChipType::kBlock},
};

}

MapChipField::IndexSet MapChipField::GetMapChipIndexByPosition(const KamataEngine::Vector3& position) const{
	MapChipField::IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + MapChipField::kBlockWidth / 2.0f) / MapChipField::kBlockWidth);

	indexSet.yIndex = static_cast<uint32_t>((MapChipField::kBlockHeight * (MapChipField::kNumBlockVirtical - 1) - position.y + MapChipField::kBlockHeight / 2.0f) / MapChipField::kBlockHeight);
	return indexSet;
}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	ResetMapChipData();

	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCsv;
	mapChipCsv << file.rdbuf();
	file.close();

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipField::MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) const{
	if (xIndex >= kNumBlockHorizontal || yIndex >= kNumBlockVirtical) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return KamataEngine::Vector3{
	    kBlockWidth * xIndex,
	    kBlockHeight * (kNumBlockVirtical - 1 - yIndex),
	    0.0f,
	};
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {

	KamataEngine::Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
	MapChipField::Rect rect;
	rect.left = center.x - MapChipField::kBlockWidth / 2.0f;
	rect.right = center.x + MapChipField::kBlockWidth / 2.0f;
	rect.top = center.y - MapChipField::kBlockHeight / 2.0f;
	rect.bottom = center.y + MapChipField::kBlockHeight / 2.0f;

	return rect;
}
#include "MapChipFIeld.h"
#include <fstream> 
#include <sstream>
#include<assert.h>
using namespace KamataEngine;
namespace {
std::unordered_map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock}
};
}

void MapChipField::ResetMapChipData() {
	// マップチップデータのリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLIne : mapChipData_.data) {
		mapChipDataLIne.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	// マップチップデータのリセット
	ResetMapChipData();
	// CSVファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());
	
	// マップチップCSV
	std::stringstream mapChipCsv;
	// 1行ずつ読み込む
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		std::string line;
		std::getline(mapChipCsv, line);
		// 1行分の文字列をストリームに変換し解析しやすくする
		std::istringstream line_stream(line);
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				// マップチップデータの設定
				mapChipData_.data[i][j] = mapChipTable[word];
			} else {
				// マップチップデータの設定
				mapChipData_.data[i][j] = MapChipType::kBlank;
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0|| kNumBlockHorizontal-1<xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0); }

MapChipField::IndexSet MapChipField::GetMapChipIndexByPosition(const KamataEngine::Vector3& position) { 
	// X座標からXインデックスを計算
	mapChipIndex_.xIndex = static_cast<uint32_t>((position.x+kBlockWidth/2)/kBlockWidth);
	// Y座標からYインデックスを計算
	mapChipIndex_.yIndex = static_cast<uint32_t>(kNumBlockVertical-1-((position.y+kBlockHeight/2)/kBlockHeight));
	return mapChipIndex_;
}

MapChipField::Rect MapChipField::GetMapChipRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 position = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = position.x - kBlockWidth / 2.0f;
	rect.right = position.x + kBlockWidth / 2.0f;
	rect.bottom = position.y - kBlockHeight / 2.0f;
	rect.top = position.y + kBlockHeight / 2.0f;
	return rect;
}

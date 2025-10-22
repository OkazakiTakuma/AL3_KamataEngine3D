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

void MapChipFIeld::ResetMapChipData() {
	// マップチップデータのリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLIne : mapChipData_.data) {
		mapChipDataLIne.resize(kNumBlockHorizontal);
	}
}

void MapChipFIeld::LoadMapChipCsv(const std::string& filePath) {
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

MapChipType MapChipFIeld::GetMapChipTypeIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0|| kNumBlockHorizontal-1<xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex];
}

Vector3 MapChipFIeld::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0); }

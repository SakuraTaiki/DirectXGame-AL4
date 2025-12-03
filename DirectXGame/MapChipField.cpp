#include "MapChipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

// 内部リンケージ
namespace {
std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
    {"2", MapChipType::kLadder},
};
}

// マップチップデータをリセット
void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);

	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//  マップチップCSV
	std::stringstream mapChipCsv;

	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();

	// マップチップデータをリセット
	ResetMapChipData();

	std::string line;

	// CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		getline(mapChipCsv, line);

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			getline(line_stream, word, ',');

			// trim front/back (spaces and CR,LF)
			auto trim = [](std::string& s) {
				// 左側
				size_t start = 0;
				while (start < s.size() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n'))
					++start;
				// 右側
				size_t end = s.size();
				while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t' || s[end - 1] == '\r' || s[end - 1] == '\n'))
					--end;
				if (start == 0 && end == s.size())
					return;
				s = s.substr(start, end - start);
			};

			trim(word);

			if (!word.empty() && mapChipTable.contains(word)) {
				mapChipData_.data[i][j] = mapChipTable[word];
			} else {
				// 規定外トークンは空白扱いに（デバッグ時のみ有効にするなど）
				mapChipData_.data[i][j] = MapChipType::kBlank;
			}
		}
	}
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }
MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex >= kNumBlockHorizontal) {
		return MapChipType::kBlank;
	}
	if (yIndex >= kNumBlockVirtical) {
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

// 02_07 スライド27枚目
MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {

	IndexSet indexSet = {};

	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2.0f) / kBlockWidth);
	indexSet.yIndex = kNumBlockVirtical - 1 - static_cast<uint32_t>((position.y + kBlockHeight / 2.0f) / kBlockHeight);

	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {

	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;

	return rect;
}

bool MapChipField::IsLadderTile(const Vector3& position) { 
	IndexSet index = GetMapChipIndexSetByPosition(position);
	MapChipType type = GetMapChipTypeByIndex(index.xIndex, index.yIndex);
	return (type == MapChipType::kLadder);
}


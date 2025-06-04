#pragma once
#include"KamataEngine.h"
using namespace KamataEngine;
enum  class MapChipType {
kBlank,
kBlock,
};
struct MapChipData {

	std::vector<std::vector<MapChipType>> data;

};


class MapChipField {

	public:

		struct IndexSet {
		    uint32_t xIndex;
		    uint32_t yIndex;
	    };

		struct Rect {
		    float left;   //左端
		    float right;  //右端
		    float bottom; //下端
		    float top;    //上端
		};

	//ブロックのサイズ

	static inline const float kBlockWidth = 1.0f;

	static inline const float kBlockHeight = 1.0f;

	//ブロックの個数

	static inline const uint32_t kNumBlockVirtical = 20; 

	static inline const uint32_t kNumBlockHorizontal = 100;

	MapChipData mapChipData_;

	//リセット関数

	void ResetMapChipData();

	

	//読み込み関数

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }

	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
};

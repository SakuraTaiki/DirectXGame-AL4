#pragma once
#include"KamataEngine.h"
#include "Player.h"
#include <vector>
#include"Math.h"
//ゲームシーン

using namespace KamataEngine;
class GameScene 
{
	
	

public:
	//初期化
	void Initialize();

	

	//更新
	void Update();

	//描画
	void Draw();
	~GameScene();

private:
	uint32_t textureHandle_ = 0;
	
	
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	
	Camera camera_;

	Player* player_ = nullptr;

	//ブロックモデル

	Model* block_model_ = nullptr;


	std::vector<std::vector<WorldTransform*>>WorldTransformBlocks_;


	Math* math_ = new Math;


	DebugCamera* debugCamera_ = nullptr;
	
	bool isDebugCameraActive_ = false;

};
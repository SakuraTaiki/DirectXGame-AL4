#pragma once
#include"KamataEngine.h"
#include "Player.h"
//ゲームシーン
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
	
	
	KamataEngine::Model* model_ = nullptr;
	
	KamataEngine::Camera camera_;
	Player* player_ = nullptr;
	
	
};
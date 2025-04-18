#pragma once
#include"KamataEngine.h"
//ゲームシーン
class GameScene 
{

public:
	//初期化
	void Initialize();
	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;

	//更新
	void Update();

	//描画
	void Draw();
};
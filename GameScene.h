#pragma once
#include"KamataEngine.h"
#include "Player.h"
//ゲームシーン
class GameScene 
{
	Player* player_ = nullptr;

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
	uint32_t soundDateHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera camera_;
	KamataEngine::DebugCamera* debugcamera_ = nullptr;
	float inputFloat3[3] = {0, 0, 0};
	
};
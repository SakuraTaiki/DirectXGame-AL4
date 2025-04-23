#pragma once
#include "KamataEngine.h"

class GameScene {
public:
	void Initialize();

	void Update();

	void Draw();

	~GameScene();

private:
	// 画像
	uint32_t textureHandle_ = 0;
	// 音声
	uint32_t soundDataHandle_ = 0;
	// 再生（音楽
	uint32_t voiceHandle_ = 0;

	float inputFloat3[3] = {0, 0, 0};

	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
};

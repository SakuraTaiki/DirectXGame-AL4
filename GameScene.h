#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>
#include "MapChipField.h"
#include "CameraController.h"
#include "Math.h"
#include "Enemy.h"

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

	void GenerateBlocks();

	void CheckAllCollisions();

private:
	////テクスチャーハンドル
	uint32_t textureHandle_ = 0;

	Sprite* sprite_ = nullptr;

	//////3Dモデル
	Model* model_ = nullptr;

	// ブロックの3Dモデル
	Model* blockModel_ = nullptr;

	WorldTransform worldTransform_;
	//
	////カメラ
	Camera camera_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

		// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	

	Model* modelskydome_ = nullptr;

	Skydome* skydome_ = nullptr;

	Model* playerModel_ = nullptr;

	//マップチップフィールド

	MapChipField* mapChipField_;

	CameraController* CameraController_;

	//// 02_09 10枚目 エネミークラス
	//Enemy* enemy_ = nullptr;
	// 02_09 10枚目 エネミーモデル
	KamataEngine::Model* enemy_model_ = nullptr;

	//エネミーリスト
	std::list<Enemy*> enemies_;
};
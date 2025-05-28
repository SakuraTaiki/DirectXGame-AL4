#include "GameScene.h"
#include"Math.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	// delete sprite_;

	delete player_;

	delete model_;

	delete blockModel_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	delete debugCamera_;

	delete modelskydome_;

	// マップチップフィールドの解放

	delete mapChipField_;
}

void GameScene::Initialize() {
	// ここにインゲームの初期化処理を書く
	

	////スプライトインスタンスの生成
	// sprite_ = Sprite::Create(textureHandle_, {100, 50});


	modelskydome_ = Model::CreateFromOBJ("skydome", true);
	
	model_ = Model::CreateFromOBJ("block",true);

	playerModel_ = Model::CreateFromOBJ("player", true);

	skydome_ = new Skydome();

	skydome_->Initialize(modelskydome_, &camera_);

	blockModel_ = Model::Create();

	debugCamera_ = new DebugCamera(1280, 720);

	// 自キャラの生成
	player_ = new Player();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(5,10);



	// 自キャラの初期化
	player_->Initialize(playerModel_, &camera_,playerPosition);

	worldTransform_.Initialize();

	camera_.Initialize();

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	CameraController_ = new CameraController();

	CameraController_->Initialize(&camera_);

	CameraController_->SetTarget(player_);

	CameraController_->Reset();
	
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};

	CameraController_->SetMovableArea(cameraArea);

}

void GameScene::Update() {
	// ここにインゲームの更新処理を書

	// 自キャラの更新
	player_->Update();
	
	skydome_->Update();

	CameraController_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// アフィン変換行列の生成
			worldTransformBlock->matWorld_ = math_->MakeAffinMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}

#ifdef _DEBUG
	// デバックの時Cキーを押すと状態が反転する
	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // ! _DEBUG
	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送AL3_02_02*/
		camera_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送AL3_02_02*/

		camera_.UpdateMatrix();
	}




}

void GameScene::Draw() {

	

	

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {

		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, camera_);
		}
	}
	player_->Draw();
	skydome_->Draw();
	Model::PostDraw();
}

void GameScene::GenerateBlocks() {
	// 要素数
	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();



	// 要素数を変更する

	worldTransformBlocks_.resize(kNumBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {

				WorldTransform* worldTransForm = new WorldTransform();

				worldTransForm->Initialize();

				worldTransformBlocks_[i][j] = worldTransForm;

				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}

		}
	}
}





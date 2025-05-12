#include"GameScene.h"
#include"imgui.h"

using namespace KamataEngine;

GameScene::~GameScene()
{ 
	
	delete model_;

	delete player_;

	delete block_model_;

delete debugCamera_;

for (std::vector<WorldTransform*>& worldTransformBlockLine : WorldTransformBlocks_) {
	for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

		delete worldTransformBlock;
	}
}
WorldTransformBlocks_.clear();

}


	void GameScene::Initialize() {

	// ここにシーンの初期化を描く

	// テクスチャハンドル
	textureHandle_ = TextureManager::Load("sample.png");

	// モデル
	model_ = Model::Create();

	// カメラのイニシャライズ
	camera_.Initialize();

	// ライン描画が参照するカメラを指定する
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// プレイヤーの初期化
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &camera_);

	// モデルの生成
	block_model_ = Model::Create();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// ワールドトランスフォームのイニシャライズ
	worldTransform_.Initialize();

	// 要素数
	const int32_t kNumBlockHorizontal = 20;
	const int32_t kNumBlockVirtical = 10;

	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数を変更する

	WorldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		WorldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if ((i + j) % 2 == 0) {
				continue;
			}
			WorldTransformBlocks_[i][j] = new WorldTransform();
			WorldTransformBlocks_[i][j]->Initialize();
			WorldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			WorldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		}
	}
}
	
//ここにシーンの更新を描く
    void GameScene::Update(){

		//プレイヤーの更新
	    player_->Update();
	    for (std::vector<WorldTransform*>&worldTransformBlockLine : WorldTransformBlocks_) {
		    for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			    if (!worldTransformBlock) 
				    continue;
				//アフィン変換行列の生成
			    worldTransformBlock->matWorld_ = math_->MakeAffinMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

			    worldTransformBlock->TransferMatrix();
		    }
		}


		//デバッグ
		#ifdef _DEBUG
	    if (Input::GetInstance()->TriggerKey(DIK_SPACE)) 
		{
		    isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif // DEBUG
	    if (isDebugCameraActive_) 
		
		{
		    debugCamera_->Update();
		    camera_.matView = debugCamera_->GetCamera().matView;
		    camera_.matProjection = debugCamera_->GetCamera().matProjection;
			//ビュープロジェクション行列の転送
		    camera_.TransferMatrix();
	    } else 
		{
	    //ビュープロジェクション行列の転送と更新
		    camera_.UpdateMatrix();
		
		}


}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ここにシーンの描画を描く
	Model::PreDraw(dxCommon->GetCommandList());
	for (std::vector<WorldTransform*>&worldTransformBlockLine : WorldTransformBlocks_)
	{
		for (WorldTransform* &worldTransformBlock : worldTransformBlockLine) 
		{
			if (!worldTransformBlock)
				continue;
				model_->Draw(*worldTransformBlock, camera_);
			
			
		}
		
	}
	Model::PostDraw();
}

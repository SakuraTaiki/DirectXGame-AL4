#include"GameScene.h"
#include"imgui.h"
using namespace KamataEngine;
GameScene::~GameScene()
{ 
	
	delete model_;
	delete player_;
}

	void GameScene::Initialize() {
//ここにシーンの初期化を描く
//テクスチャハンドル
textureHandle_ = TextureManager::Load("sample.png");
//サウンドハンドル

//モデル
model_ = Model::Create();
camera_.Initialize();
//ライン描画が参照するカメラを指定する
PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

//プレイヤーの初期化
player_ = new Player();
player_->Initialize(model_,textureHandle_,&camera_);

    }

void GameScene::Update() {
//ここにシーンの更新を描く
		//プレイヤーの更新
	    player_->Update();


}

void GameScene::Draw() {
	// ここにシーンの描画を描く
	player_->Draw();
}

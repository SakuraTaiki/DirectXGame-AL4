#include"GameScene.h"
using namespace KamataEngine;

void GameScene::Initialize() {
//ここにシーンの初期化を描く
	textureHandle_ = TextureManager::Load("mario.jpg");
sprite_ = Sprite::Create(textureHandle_, {100, 50});
	delete sprite_;
}

void GameScene::Update() {
//ここにシーンの更新を描く
	Vector2 position = sprite_->GetPosition();
	//座標を(2,1)へ移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
}

void GameScene::Draw() {
	// ここにシーンの描画を描く

	//DirectXCommonのインスタンス取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	//スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	//	スプライト描画後処理
	Sprite::PostDraw();
}

#include"GameScene.h"
#include"imgui.h"
using namespace KamataEngine;
GameScene::~GameScene()
{ 
	delete sprite_;
	delete model_;
	delete debugcamera_;
	delete player_;
}

	void GameScene::Initialize() {
//ここにシーンの初期化を描く
//テクスチャハンドル
textureHandle_ = TextureManager::Load("sample.png");
//サウンドハンドル
soundDateHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
//オーディオ
Audio::GetInstance()->PlayWave(soundDateHandle_);
voiceHandle_ = Audio::GetInstance()->PlayWave(soundDateHandle_, true);
//スプライト生成
sprite_ = Sprite::Create(textureHandle_, {100, 50});
//モデル
model_ = Model::Create();
worldTransform_.Initialize();
camera_.Initialize();
//ライン描画が参照するカメラを指定する
PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
//デバッグカメラの生成
debugcamera_ = new DebugCamera(1280, 720);
//軸方向表示の表示を有効にする
AxisIndicator::GetInstance()->SetVisible(true);
AxisIndicator::GetInstance()->SetTargetCamera(&debugcamera_->GetCamera());
//プレイヤーの初期化
player_ = new Player();
player_->Initialize(model_,textureHandle_,&camera_);

    }

void GameScene::Update() {
//ここにシーンの更新を描く
	Vector2 position = sprite_->GetPosition();
	//座標を(2,1)へ移動
	position.x += 2.0f;
	position.y += 1.0f;
	//移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	debugcamera_->Update();
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceHandle_);
	}
		#ifdef _DEBUG
		ImGui::Begin("Debug1");
		ImGui::InputFloat3("InputFloat3", inputFloat3);
		ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
		ImGui::Text("Sakura Taiki%d.%d.%d", 2005, 12, 04);
		ImGui::End();
		ImGui::ShowDemoWindow();
		#endif

		//プレイヤーの更新
	    player_->Update();


}

void GameScene::Draw() {
	// ここにシーンの描画を描く

	//DirectXCommonのインスタンス取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, debugcamera_->GetCamera(), textureHandle_);
	//	スプライト描画後処理
	Sprite::PostDraw();
	Model::PostDraw();
	//ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d({0,0,0,},{0,10,0},{1.0f,0.0f,0.0f,1.0f});
	//プレイヤーの描画
	player_->Draw();
}

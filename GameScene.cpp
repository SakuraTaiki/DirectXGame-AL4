#include "GameScene.h"
#include "ImGui.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	textureHandle_ = TextureManager::Load("sample.png");
	textureHandle_ = TextureManager::Load("sample.png");

	soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);

	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	// デバッグカメラの生成                          横幅、縦幅
	debugCamera_ = new DebugCamera(30, 30);

	worldTransform_.Initialize();
	camera_.Initialize();

	Audio::GetInstance()->PlayWave(soundDataHandle_);

	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		Audio::GetInstance()->StopWave(voiceHandle_);
	}
	ImGui::Begin("Debug1");

	// float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);

	// float3スレンダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
#ifdef _DEBUG
	ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
#endif

	ImGui::End();

	ImGui::ShowDemoWindow();

	debugCamera_->Update();
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();

	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	Model::PostDraw();
	//                                        　始点座標  終点座標　　　色(RGBA
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
}

#include "Math.h"
#include "GameOver.h"
#include <numbers>

GameOver::~GameOver() {
	
	delete spriteGameOver_;

	delete fade_;
}

void GameOver::Initialize() {

	textureHandle_ = TextureManager::Load("GameOver.png");

	spriteGameOver_ = Sprite::Create(textureHandle_, {640.0f, 360.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	fade_ = new Fade();

	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	

	
}

void GameOver::Update() {

	//// 02_12 27枚目
	// if (Input::GetInstance()->PushKey(DIK_SPACE)) {
	//	finished_ = true;
	// }

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	
}

void GameOver::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	spriteGameOver_->Draw();
	
	fade_->Draw();
	Sprite::PostDraw();
}
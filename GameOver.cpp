#include "Math.h"
#include "GameOver.h"
#include <numbers>

GameOver::~GameOver() {
	
	delete modelGameOver_;

	delete fade_;
}

void GameOver::Initialize() {

	modelGameOver_ = Model::CreateFromOBJ("GameOver", true);

	// カメラ初期化
	camera_.Initialize();

	fade_ = new Fade();

	fade_->Initialize();

	// 02_13 22枚目
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	const float kPlayerTitle = 7.0f;

	worldTransformGameOver_.Initialize();

	worldTransformGameOver_.scale_ = {kPlayerTitle, kPlayerTitle, kPlayerTitle};
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

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeGameOverMove);

	float angle = counter_ / kTimeGameOverMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformGameOver_.translation_.y = std::sin(angle) + 10.0f;

	camera_.TransferMatrix();

	// アフィン変換～DirectXに転送(タイトル座標)
	WorldTransformUpdate(worldTransformGameOver_);

	

	fade_->Update();
}

void GameOver::Draw() {

	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelGameOver_->Draw(worldTransformGameOver_, camera_);
	
	fade_->Draw();
	Model::PostDraw();
}
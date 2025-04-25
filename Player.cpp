#include "Player.h"
void Player::Initialize(KamataEngine::Model * model, uint32_t textureHandle, KamataEngine::Camera* camera) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	camera_ = camera;

}


void Player::Update() { 
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();
	KamataEngine::Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, *camera_, textureHandle_); 
	KamataEngine::Model::PostDraw();
}

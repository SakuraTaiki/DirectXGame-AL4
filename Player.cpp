#include "Player.h"
#include"cassert"
using namespace KamataEngine;
void Player::Initialize(KamataEngine::Model * model, uint32_t textureHandle, KamataEngine::Camera* camera) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	camera_ = camera;

}


void Player::Update() { 
worldTransform_.matWorld_ = math->MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Player::Draw() { model_->Draw(worldTransform_, *camera_);

	
}

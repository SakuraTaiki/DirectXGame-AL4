#include "Skydome.h"

void Skydome::Initialize(Model* model, Camera* camera) {
	assert(model);
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
}

void Skydome::Update() 
{ 
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix(); 
}

void Skydome::Draw()
{


model_->Draw(worldTransform_, *camera_);
	

}

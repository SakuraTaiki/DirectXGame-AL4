#include "Player.h"
#include"cassert"
using namespace KamataEngine;

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position)
{
	assert(model);

	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	//座標をマップチップ番号で指定



	camera_ = camera;
}

void Player::Update() {
	

	//移動入力
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
	//左右移動

		Vector3 acceleration = {};

		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			acceleration.x += kAccelerration;
		}

	
	}

	//移動
	worldTransform_.translation_.x += velocity_.x;

	worldTransform_.translation_.y += velocity_.y;

	worldTransform_.translation_.z += velocity_.z;


	//行列更新
	worldTransform_.matWorld_ = math->MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();


}

void Player::Draw() { model_->Draw(worldTransform_, *camera_);

	
}

#include "Player.h"
#include"cassert"
#include <algorithm>
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

			//左移動中に右入力
			if (velocity_.x < 0.0f) {

			//速度と逆方向に入力中は急ブレーキ

				velocity_.x *= (1.0f - kAttenuation);
			}


			//向き（右）
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
			}

			acceleration.x += kAccelerration;

		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) 
		{
			//右移動中の左入力

			if (velocity_.x > 0.0f) {
			
			//速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}

			//向き（左）

			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
			}

			acceleration.x -= kAccelerration;

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		velocity_ += acceleration;
	
		//最大速度制限

		velocity_.x =
			std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

		//旋回制限
		{
			float destinationRotationYTable[] = {
				std::numbers::pi_v<float>/2.0f,
				std::numbers::pi_v<float>*3.0f/2.0f
			};
			//状態に応じた角度を取得

			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

			//自キャラの角度を設定する

			worldTransform_.rotation_.y = destinationRotationY;

		}

	}

	//移動
	worldTransform_.translation_ += velocity_;

	


	//行列更新
	worldTransform_.matWorld_ = math->MakeAffinMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();


}

void Player::Draw() { model_->Draw(worldTransform_, *camera_);

	
}

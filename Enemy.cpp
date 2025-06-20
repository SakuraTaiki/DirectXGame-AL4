#include "Enemy.h"
#include "Math.h"
#include <cassert>
#include <numbers>

// 02_09 スライド5枚目
void Enemy::Initialize(Model* model, Camera* camera, const Vector3& position) {

	// NULLチェック
	assert(model);

	// 02_09 7枚目
	model_ = model;
	// 02_09 7枚目
	camera_ = camera;
	// 02_09 7枚目
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 02_09 7枚目 角度調整
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;

}
void Enemy::Update() 
{
	//02_09 16枚目

	worldTransform_.translation_ += velocity_;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

		// 02_09 23枚目 回転アニメーション
	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	float degree = kWallMotionAngleStart + kWallMotionAngleEnd * (param + 1.0f) / 2.0f;

	worldTransform_.rotation_.x = degree * (float(std::numbers::pi_v<float> / 180.0f));

	WorldTransformUpdate(worldTransform_);
}
void Enemy::Draw() 
{
	// 02_09 スライド9枚目  モデル描画
	model_->Draw(worldTransform_, *camera_);
};
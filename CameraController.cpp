#include <algorithm>
#include "CameraController.h"
#include"Player.h"
#include"Math.h"


void CameraController::Initialize(Camera*camera) 
{ camera_ = camera;

}

void CameraController::Update() 
{
//追従対象のワールドトランスフォームを参照

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();

	//追従対象とオフセットからカメラの座標を計算

	destination_ = targetWorldTransform.translation_ + targetOffset_+targetVelocity*kVelocityBias;

	//座標補間によりゆったり追従

	camera_->translation_ =Lerp(camera_->translation_, destination_, kInterpolationRate);

	//移動範囲制限

	camera_->translation_.x = max(camera_->translation_.x, movableArea_.left);
	
	camera_->translation_.x = max(camera_->translation_.x, movableArea_.right);

	camera_->translation_.y = max(camera_->translation_.y, movableArea_.bottom);

	camera_->translation_.y = max(camera_->translation_.y, movableArea_.top);

	//行列を更新する

	camera_->UpdateMatrix();

}

void CameraController::Reset() 
{
	//追従対象のワールドトランスフォームを参照

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//追従対象とオフセットからカメラの座標の計算

	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}

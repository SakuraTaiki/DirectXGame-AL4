#include "Bullet.h"
Bullet* Bullet::Create(Model* model, Camera* camera, const Vector3& position, const Vector3& velocity) {
	Bullet* bullet = new Bullet();
	bullet->Initialize(model, camera, position, velocity);
	return bullet;
}
void Bullet::Initialize(Model* model, Camera* camera, const Vector3& position, const Vector3& velocity) {
	model_ = model;
	camera_ = camera;
	velocity_ = velocity;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}
void Bullet::Update() {
	worldTransform_.translation_ += velocity_;
	WorldTransformUpdate(worldTransform_);

	lifeTime_--;
	if (lifeTime_ <= 0) {
		isDead_ = true;
	}
}
void Bullet::Draw() { model_->Draw(worldTransform_, *camera_); }
AABB Bullet::GetAABB() const {
	Vector3 pos = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]};

	AABB aabb;
	aabb.min = {pos.x - kRadius, pos.y - kRadius, pos.z - kRadius};
	aabb.max = {pos.x + kRadius, pos.y + kRadius, pos.z + kRadius};
	return aabb;
}
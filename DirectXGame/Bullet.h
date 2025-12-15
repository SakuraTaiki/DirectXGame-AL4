#pragma once
#include"Player.h"
class Bullet 
{
public:
	Bullet* Create(Model* model, Camera* camera, const Vector3& position, const Vector3& velocity);
	void Initialize(Model* model, Camera* camera, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw();

	bool IsDead() const { return isDead_; }
	AABB GetAABB() const;

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;
	Vector3 velocity_{};

	int32_t lifeTime_ = 120;
	bool isDead_ = false;

	 static constexpr float kRadius = 0.3f;
};

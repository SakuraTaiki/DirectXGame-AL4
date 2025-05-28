#pragma once
#include <KamataEngine.h>
#include"Math.h"

class Player {
public:
	void Initialize(KamataEngine::Model*model,KamataEngine::Camera*camera,const Vector3& position);

	void Update();

	void Draw();

	enum class LRDirection {
		kRight,
		kLeft,
	};

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始の角度

	float turnFirstRotationY_ = 0.0f;

	//	旋回タイマー

	float turnTimer_ = 0.0f;


	Vector3 velocity_ = {};

	//フレームごとの加速度

	static inline const float kAcceleration = 0.01f;

	//非入力時の摩擦係数

	static inline const float kAttenuation = 0.05f;

	static inline const float kLimitRunSpeed = 0.3f;


	//旋回時間
	static inline const float kTimeTurn = 0.3f;

	//着地状態フラグ

	bool onGround_ = true;

	//重力加速度

	static inline const float kGravityAcceleration = 0.2f;


	//最大落下速度

	static inline const float kLimitFallSpeed = 0.2f;

	//ジャンプ初速

	static inline const float kJumpAcceleration = 10.0f;


	Math* math;
};
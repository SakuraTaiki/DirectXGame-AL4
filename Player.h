#pragma once
#include <KamataEngine.h>
#include"Math.h"
#include<numbers>
class Player {
public:
	void Initialize(KamataEngine::Model*model,KamataEngine::Camera*camera,const Vector3& position);

	void Update();

	void Draw();

	enum class LRDirection {
		kRight,
		kLeft,
	};

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;

	LRDirection lrDirection_ = LRDirection::kRight;


	Vector3 velocity_ = {};

	//フレームごとの加速度

	static inline const float kAccelerration = 0.01f;

	//非入力時の摩擦係数

	static inline const float kAttenuation = 0.05f;

	static inline const float kLimitRunSpeed = 0.3f;

	Math* math;
};
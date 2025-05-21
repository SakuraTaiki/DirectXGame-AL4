#pragma once
#include <KamataEngine.h>
#include"Math.h"
#include<numbers>
class Player {
public:
	void Initialize(KamataEngine::Model*model,KamataEngine::Camera*camera,const Vector3& position);
	void Update();
	void Draw();

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	uint32_t textureHandle_ = 0u;



	Vector3 velocity_ = {};

	static inline const float kAccelerration = 0.01f;

	static inline const float kAttenuation = 0.01f;

	Math* math;
};
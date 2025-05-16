#pragma once
#include "KamataEngine.h"
#include "math.h"
using namespace KamataEngine;
class Skydome {
public:
	void Initialize(Model* model, Camera * camera);

	void Update();

	void Draw();

	

	private:
	WorldTransform worldTransform_;
	    Camera* camera_ = nullptr;
	Model* model_ = nullptr;
	    Math* math;
};

#pragma once

#include "KamataEngine.h"

using namespace KamataEngine;

class Enemy {
public:
	// 02_09 スライド5枚目
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	// 02_09 スライド5枚目
	void Update();
	// 02_09 スライド5枚目
	void Draw();

private:
	// 02_09 6枚目 ザ・ワールド
	WorldTransform worldTransform_;
	// 02_09 6枚目 モデル
	Model* model_ = nullptr;
	// 02_09 6枚目 カメラ
	Camera* camera_ = nullptr;

	//敵の歩行の速さ
	static inline const float kWalkSpeed = 0.02f;
	//速度
	Vector3 velocity_ = {};

	//最初の角度
	static inline const float kWallMotionAngleStart = -10.0f;

	//最後の角度
	static inline const float kWallMotionAngleEnd = 40.0f;

	//アニメーション周期
	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer_ = 0.0f;

};

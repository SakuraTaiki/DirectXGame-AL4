#pragma once
#include <KamataEngine.h>
#include"Math.h"

class MapChipField;

class Player {
public:
	void Initialize(KamataEngine::Model*model,KamataEngine::Camera*camera,const Vector3& position);

	void Update();

	void Draw();

	enum class LRDirection {
		kRight,
		kLeft,
	};

	enum Corner {
		kRightBottom,//右下
		kLeftBottom, //左下
		kRightTop,   //右上
		kLeftTop,    //左上
		kNumCorner   //要素数
	};

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const KamataEngine::Vector3& GetVelocity() const { return velocity_; } 

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField;
	}

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	MapChipField* mapChipField_ = nullptr;
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

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;


	void InputMove();

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);
};
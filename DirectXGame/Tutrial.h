#pragma once
#include "Fade.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class Tutrial 
{
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~Tutrial();

	void Initialize();

	void Update();

	void Draw();

	// 02_12 26枚目
	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeTutrial = 2.0f;

	// ビュープロジェクション

	Fade* fade_ = nullptr;

	Sprite* spriteTutrial_ = nullptr;

	uint32_t textureHandle_ = 0;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};

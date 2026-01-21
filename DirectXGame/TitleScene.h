#pragma once
#include "Fade.h"
#include "KamataEngine.h"

using namespace KamataEngine;

// 02_12 19枚目 タイトルシーン
class TitleScene {
public:
	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	// 02_12 26枚目
	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeTitleMove = 2.0f;

	// ビュープロジェクション
	
	
	Sprite* spriteTitle_ = nullptr;

	uint32_t textureHandle_ = 0;

	Fade* fade_ = nullptr;

	float counter_ = 0.0f;
	// 02_12 26枚目
	bool finished_ = false;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};
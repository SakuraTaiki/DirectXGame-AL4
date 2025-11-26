#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;
class Fade {
public:
	enum class Status {
		None,
		FadeIn,
		FadeOut,
	};

	void Initialize();
	void Update();
	void Draw();

	// 02_13 18枚目 フェード開始
	void Start(Status status, float duration);

	// 02_13 24枚目 フェード停止
	void Stop();

	// 02_13 26枚目 フェード終了判定
	bool IsFinished() const;

private:
	Sprite* sprite_ = nullptr;

	// 02_13 16枚目 現在のフェードの状態
	Status status_ = Status::None;

	float duration_ = 0.0f;
	float counter_ = 0.0f;
};
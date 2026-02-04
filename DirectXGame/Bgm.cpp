#include "Bgm.h"
#include "KamataEngine.h"
#include <stdio.h>

using namespace KamataEngine;

void BGM::Initialize() {
	bgmHandle_ = 0;
	isPlaying_ = false;
}

void BGM::BGMPlay(uint32_t bgmHandle) {
	if (isPlaying_) {
		return;
	}
	bgmHandle_ = Audio::GetInstance()->PlayWave(bgmHandle, true);
	isPlaying_ = true;
}

void BGM::BGMStop() {
	if (!isPlaying_) {
		return;
	}
	Audio::GetInstance()->StopWave(bgmHandle_);
	bgmHandle_ = 0;
	isPlaying_ = false;
}

bool BGM::IsPlaying() const {
	return isPlaying_; 
}
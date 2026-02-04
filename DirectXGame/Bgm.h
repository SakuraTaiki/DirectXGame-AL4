#pragma once
#include<array>
namespace KamataEngine {
class Audio;
}
class BGM {
public:
	void Initialize();
	void BGMPlay(uint32_t bgmHandle);
	void BGMStop();
	bool IsPlaying() const;

private:
	uint32_t bgmHandle_ = 0;
	bool isPlaying_ = false; // ★これを追加
};
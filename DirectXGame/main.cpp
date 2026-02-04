#include "Enemy.h"
#include "GameClear.h"
#include "GameOver.h"
#include "GameScene.h"
#include "KamataEngine.h"
#include "Player.h"
#include "TitleScene.h"
#include "Tutrial.h"
#include <Windows.h>
#include <fstream>
#include <mmsystem.h>
#include <wrl.h>
#include "Bgm.h"
#pragma comment(lib, "winmm.lib")

using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)

// ゲームシーンのインスタンス生成
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
Tutrial* tutrialScene = nullptr;
GameClear* gameClearScene = nullptr;
GameOver* gameOverScene = nullptr;
Enemy* enemy = nullptr;
Player* player = nullptr;

enum class Scene {
	kUnknown = 0,
	kTitle,
	kTutrial,
	kGame,
	kClear,
	kOver,
};

void ChangeScene();

void UpdateScene();

void DrawScene();

BGM* bgm_ = nullptr;

// BGM
//   タイトルBGM

Scene scene = Scene::kUnknown;

uint32_t gameClearBgmHandle_;
uint32_t gamePlayBgmHandle_;
uint32_t titleBgmHandle_;
uint32_t overBgm_;


int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_15_サクラ_タイキ_氷結大探索");

	
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// タイトル
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	gameClearBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/Clear.mp3");
	gamePlayBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/GamePlay.mp3");
	titleBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/Title.mp3");
	overBgm_ = Audio::GetInstance()->LoadWave("./BGM/GameOver.mp3");

	bgm_ = new BGM();
	bgm_->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		ChangeScene();

		UpdateScene();


		// imGui受付開始
		imguiManager->Begin();

		

		// シーン更新

		// imGui受付終了
		imguiManager->End();
		// 描画開始
		dxCommon->PreDraw();

		DrawScene();

		// 軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		// プリミティブ描画のリセット
		PrimitiveDrawer::GetInstance()->Reset();

		// imGui描画
		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの開放
	delete gameScene;

	// タイトルの開放
	delete titleScene;

	delete tutrialScene;

	delete gameClearScene;

	delete gameOverScene;

	delete enemy;

	delete player;


	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}

void ChangeScene() {

	switch (scene) {
	case Scene::kTitle:
		if (!bgm_->IsPlaying())
			bgm_->BGMPlay(titleBgmHandle_);
		if (titleScene->IsFinished()) {
			scene = Scene::kTutrial;
			delete titleScene;
			titleScene = nullptr;
			tutrialScene = new Tutrial;
			tutrialScene->Initialize();
		}
		break;
	case Scene::kTutrial:
		if (tutrialScene->IsFinished()) {
			bgm_->BGMStop();
			scene = Scene::kGame;
			delete tutrialScene;
			tutrialScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (!bgm_->IsPlaying()) {
			bgm_->BGMPlay(gamePlayBgmHandle_);
		}

		// クリア
		if (gameScene->AreAllEnemiesDefeated()) {
			bgm_->BGMStop();

			scene = Scene::kClear;
			delete gameScene;
			gameScene = nullptr;
			gameClearScene = new GameClear;
			gameClearScene->Initialize();
			break;
		}
		
		
		// ゲームオーバー
		if (gameScene->GetPlayer()->IsDead()) {
			bgm_->BGMStop();

			scene = Scene::kOver;
			gameOverScene = new GameOver;
			gameOverScene->Initialize();

			delete gameScene;
			gameScene = nullptr;
			break; 
		}

		break;
	case Scene::kClear:
		if (!bgm_->IsPlaying())
			bgm_->BGMPlay(gameClearBgmHandle_);
		if (gameClearScene->IsFinished()) {
			bgm_->BGMStop();
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kOver:
		if (!bgm_->IsPlaying())
			bgm_->BGMPlay(overBgm_);
		if (gameOverScene->IsFinished()) {
			bgm_->BGMStop();
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kTutrial:
		tutrialScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		gameClearScene->Update();
		break;
	case Scene::kOver:
		gameOverScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kTutrial:
		tutrialScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		gameClearScene->Draw();
		break;
	case Scene::kOver:
		gameOverScene->Draw();
		break;
	}
}
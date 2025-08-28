#include "GameScene.h"
#include"Player.h"
#include"Enemy.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include"GameClear.h"
#include"GameOver.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)

// ゲームシーンのインスタンス生成
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
GameClear* gameClearScene = nullptr;
GameOver* gameOverScene = nullptr;
Enemy* enemy = nullptr;
Player* player = nullptr;

enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kClear,
	kOver,
};

//BGM
//  タイトルBGM
void PlayTitleBGM() { PlaySound(TEXT("Title.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); }


// ゲームプレイBGM
void PlayGameBGM() { PlaySound(TEXT("PlayGame.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); }

// ゲームオーバーBGM
void PlayGameOverBGM() { PlaySound(TEXT("GameOver.wav"), NULL, SND_FILENAME | SND_ASYNC); }

// クリアBGM
void PlayClearBGM() { PlaySound(TEXT("GameClear.wav"), NULL, SND_FILENAME | SND_ASYNC); }

// BGMを止める
void StopBGM() { PlaySound(NULL, 0, 0); }

bool isBGMPlaying = false;

Scene scene = Scene::kUnknown;

void ChangeScene() {

	switch (scene) {

	case Scene::kTitle:
		if (!isBGMPlaying) {
			PlayTitleBGM();
			isBGMPlaying = true;
		}
		if (titleScene->IsFinished()) {
			StopBGM();
			isBGMPlaying = false;
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (!isBGMPlaying) {
			PlayGameBGM();
			isBGMPlaying = true;
		}
		if (gameScene->GetPlayer()->IsDead()) {
			StopBGM();
			isBGMPlaying = false;
			scene = Scene::kOver;
			delete gameScene;
			gameScene = nullptr;
			gameOverScene = new GameOver;
			gameOverScene->Initialize();
		} else if (gameScene->AreAllEnemiesDefeated()) {
			StopBGM();
			isBGMPlaying = false;
			scene = Scene::kClear;
			delete gameScene;
			gameScene = nullptr;
			gameClearScene = new GameClear;
			gameClearScene->Initialize();
		}

			 // Input クラスで ESC キー押下を判定
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			StopBGM();
			isBGMPlaying = false;

			if (scene == Scene::kGame) {
				scene = Scene::kTitle;
				if (!isBGMPlaying) {
					PlayTitleBGM();
					isBGMPlaying = true;
				}
				
				delete gameScene;
				gameScene = nullptr;

				titleScene = new TitleScene;
				titleScene->Initialize();

				return; // Update終了
			}
		}
		break;
	case Scene::kClear:

		if (!isBGMPlaying) {
			PlayClearBGM();
			isBGMPlaying = true;
		}
		if (gameClearScene->IsFinished()) {
			StopBGM();
			isBGMPlaying = false;
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;

	case Scene::kOver:

		 if (!isBGMPlaying) {
			PlayGameOverBGM();
			isBGMPlaying = true;
		}
		if (gameOverScene->IsFinished()) {
			StopBGM();
			isBGMPlaying = false;
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

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_10_サクラ_タイキ_AL3");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// タイトル
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}
		// imGui受付開始
		imguiManager->Begin();

		UpdateScene(); // 02_12 33枚目で追加


		// シーン切り替え
		ChangeScene(); // 02_12 33枚目で追加
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
		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの開放
	delete gameScene;

	// タイトルの開放
	delete titleScene;

	delete gameClearScene;

	delete gameOverScene;

	delete enemy;

	delete player;
	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
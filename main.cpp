#include "GameScene.h"
#include"Player.h"
#include"Enemy.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include"GameClear.h"
#include"GameOver.h"
#include <Windows.h>

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

Scene scene = Scene::kUnknown;

void ChangeScene() {

	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {

			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();


		}
		break;
	case Scene::kGame:
		// 02_12 30枚目
	
			// シーン変更
			if (gameScene->GetPlayer()->IsDead()) {
				scene = Scene::kOver;
				delete gameScene;
				gameScene = nullptr;
				gameOverScene = new GameOver;
				gameOverScene->Initialize();
			} else if (gameScene->AreAllEnemiesDefeated()) {
				scene = Scene::kClear;
				delete gameScene;
				gameScene = nullptr;
				gameClearScene = new GameClear;
				gameClearScene->Initialize();
			
		}
		break;
	case Scene::kClear:
		if (gameClearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kOver:
		if (gameOverScene->IsFinished()) {
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
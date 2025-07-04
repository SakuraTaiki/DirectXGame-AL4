#include "GameScene.h"
#include"TitleScene.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)

	// ゲームシーンのインスタンス生成
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

enum class Scene { 
	kUnknown = 0,
	kTitle,
	kGame,
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
		if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			delete gameScene;
			gameScene = nullptr;
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
		
		// シーン切り替え
		ChangeScene(); // 02_12 33枚目で追加
		// シーン更新
		UpdateScene(); // 02_12 33枚目で追加

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

	//タイトルの開放
	delete titleScene;
	
	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
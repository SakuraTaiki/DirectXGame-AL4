#include "GameScene.h"
#include "kamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"LE2C_10_サクラ_タイキ_AL3");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	GameScene* gameScene = new GameScene();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	gameScene->Initialize();

	while (true) {

		if (KamataEngine::Update()) {
			break;
		}
		// Imgui受付開始
		imguiManager->Begin();

		// ゲームシーンの更新
		gameScene->Update();

		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();

		// ゲームシーンの描画
		gameScene->Draw();
		// 軸表示の描画
		AxisIndicator::GetInstance()->Draw();
		// ImGui描画
		imguiManager->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete gameScene;

	gameScene = nullptr;
	KamataEngine::Finalize();

	return 0;
}
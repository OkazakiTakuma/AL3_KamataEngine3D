#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	ImGuiManager* imguiMneger = ImGuiManager::GetInstance();
	Initialize(L"LD2B_01_オカザキ_タクマ_AL3");
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	while (true) {
		if (Update()) {	
			break;
		}
		//// 更新処理 ////
		
		// IMguiの更新処理
		imguiMneger->Begin();
		// ゲームシーンの更新処理
		gameScene->Update();

		imguiMneger->End();

		//// 描画処理 ////

		dxCommon->PreDraw();
		
		// ゲームシーンの描画処理
		gameScene->Draw();

		// 軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		// Imguiの描画処理
		imguiMneger->Draw();

		dxCommon->PostDraw();
	}
	Finalize();
	delete gameScene;
	return 0;
}

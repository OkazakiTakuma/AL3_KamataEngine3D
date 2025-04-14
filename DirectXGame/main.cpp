#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	Initialize(L"LD2B_01_オカザキ_タクマ_AL3");
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	while (true) {
		if (Update()) {	
			break;
		}
		//テスト用
		gameScene->Update();

		dxCommon->PreDraw();

		gameScene->Draw();

		dxCommon->PostDraw();
	}
	Finalize();
	delete gameScene;
	return 0;
}

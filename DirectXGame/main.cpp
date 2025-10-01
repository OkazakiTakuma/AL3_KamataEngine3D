#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;
TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;

enum Scene {
	kNull,
	kTitle,
	kGame,
};
Scene scene = Scene::kNull;
void ChangeScene();
void UpdateScene();
void DrawScene();
    // Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	ImGuiManager* imguiMneger = ImGuiManager::GetInstance();
	Initialize(L"LD2B_01_オカザキ_タクマ_AL3");
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	while (true) {
		if (Update()) {
			break;
		}
		//// 更新処理 ////

		// IMguiの更新処理
		imguiMneger->Begin();
		
		ChangeScene();

		UpdateScene();

		imguiMneger->End();

		//// 描画処理 ////

		dxCommon->PreDraw();
		DrawScene();

		// 軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		// Imguiの描画処理
		imguiMneger->Draw();

		dxCommon->PostDraw();
	}
	Finalize();
	delete titleScene;
	delete gameScene;
	return 0;
}
void ChangeScene() {
	switch (scene) {
	case kNull:
		break;
	case kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case kGame:
		if (gameScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameScene;
			gameScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	default:
		break;
	}
};

void UpdateScene() {
	switch (scene) {
	case kNull:
		break;
	case kTitle:
		// タイトルシーンの更新処理
		titleScene->Update();
		break;
	case kGame:
		// ゲームシーンの更新処理
		gameScene->Update();
		break;
	default:
		break;
	}
}
void DrawScene() {
	switch (scene) {
	case kNull:
		break;
	case kTitle:
		// タイトルシーンの描画処理
		titleScene->Draw();
		break;
	case kGame:
		// ゲームシーンの描画処理
		gameScene->Draw();
		break;
	default:
		break;
	}
}
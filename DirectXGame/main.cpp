#include "GameScene.h"

#include "TitleScene.h"
using namespace KamataEngine;

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
#endif*/
	
	titleScene = new TitleScene();
	// タイトルシーンの初期化
	titleScene->Initialize();


	//メインループ
	while (true) {


		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		imguiManager->Begin();

		


		ChangeScene();
		UpdataScene();

		

		//ImGui受付終了
		imguiManager->End();

		//描画開始
		dxCommon->PreDraw();



		DrawScene();
		
		//軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		//ImGui描画
		imguiManager->Draw();

		//描画終了
		dxCommon->PostDraw();

		if (KamataEngine::Input::GetInstance()->PushKey(DIK_ESCAPE)) {
			return 0; // 左キーが押されたら終了
		}

	}


	// ゲームシーンの解放

	delete gameScene;
	delete titleScene;
	//nullptrの代入
	gameScene = nullptr;

	KamataEngine::Finalize();

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
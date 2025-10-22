#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
using namespace KamataEngine;

enum class Scene {

	kUnknown=0, // 不明なシーン

	kTitle, // タイトルシーン
	kGame,  // ゲームシーン
};


Scene scene = Scene::kUnknown; // 現在のシーン

GameScene* gameScene = nullptr; // ゲームシーンのインスタンス

TitleScene* titleScene = nullptr;

void ChangeScene() {
	switch (scene) {
	
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
		//シーン変更
			scene = Scene::kGame; // タイトルシーンが終了したらゲームシーンに変更

			delete titleScene; // タイトルシーンのインスタンスを解放
			titleScene = nullptr;
			// ゲームシーンの初期化
			                      // ゲームシーンのインスタンス生成
			gameScene = new GameScene();
			// ゲームシーンの初期化
			gameScene->Initialize();
		} 

		break;
	case Scene::kGame:

		if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle; // ゲームシーンが終了したらタイトルシーンに変更
			delete gameScene;      // ゲームシーンのインスタンスを解放
			gameScene = nullptr;
			// タイトルシーンの初期化
			titleScene = new TitleScene();
			titleScene->Initialize();
		}


		break;
	default:
		break;
	}



}

void UpdataScene() {
	switch (scene) {
	case Scene::kTitle:
		// タイトルシーンの更新
		titleScene->Update();
		break;
	case Scene::kGame:
		// ゲームシーンの更新
		gameScene->Update();
		break;


	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		// タイトルシーンの描画
		titleScene->Draw();
		break;
	case Scene::kGame:
		// ゲームシーンの描画
		gameScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	//エンジンの初期化
	KamataEngine::Initialize(L"LE2B_25_ミヤザワ_ハルヒ_TR");

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	
	

	scene = Scene::kTitle; // シーンをタイトルに設定

	/*#ifdef _DEBUG
	scene = Scene::kGame;

	gameScene = new GameScene();
	gameScene->Initialize();
#else
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

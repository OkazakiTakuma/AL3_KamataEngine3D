#include "GameScene.h"
using namespace KamataEngine;

void GameScene::Initialize() {
#pragma region 画像・3Dモデル生成
	tecstureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(tecstureHandle_, {100, 50});
	// 3Dモデルの作成
	model_ = Model::Create();
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクション
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());
#pragma endregion

#pragma region 音声生成
	// サウンドデータの生成
	soundDataHandle_ = Audio::GetInstance()->LoadWave("BGM.wav");
	// 音声再生
	voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
#pragma endregion
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Update() {
	Vector2 postion = sprite_->GetPosition();
	postion.x += 2.0f;
	postion.y += 2.0f;
	sprite_->SetPosition(postion);

#pragma region 音声
	// スペースキーを押した瞬間
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声停止
		Audio::GetInstance()->StopWave(voiceHandle_);
	}
#pragma endregion

#ifdef _DEBUG
	// デバッグテキストの表示
	ImGui::Text("Okazaki Takuma %d.%d.%d", 2050, 12, 31);
	ImGui::Begin("Debug1");
	// float3入力ボックス
	ImGui::InputFloat3("Inputfloat3", inputFloat3);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();
	// デモウィンドウの表示の有効化
	ImGui::ShowDemoWindow();
	// デバッグカメラの更新
	debugCamera_->Update();

#endif
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// スプライトの描画
	Sprite::PreDraw(dxCommon->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
	// 3Dモデルの描画
	Model::PreDraw(dxCommon->GetCommandList());
	model_->Draw(worldTransform_, debugCamera_->GetCamera(), tecstureHandle_);
#ifdef _DEBUG
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 10}, {1.0f, 0.0f, 0.0f, 1.0f});
#endif

	Model::PostDraw();
}

#include "GameScene.h"
using namespace KamataEngine;

void GameScene::Initialize() {
	// カメラの初期化
	camera_.Initialize();
#pragma region 自キャラの初期化
	playerTecstureHandle_ = TextureManager::Load("uvChecker.png");
	playerSprite_ = Sprite::Create(playerTecstureHandle_, {100, 50});
	// 3Dモデルの作成
	playerModel_ = Model::Create();
	// ワールドトランスフォームの初期化
	playerWorldTransform_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(playerModel_,playerTecstureHandle_,&camera_);

#pragma endregion
	
}

GameScene::~GameScene() {
	delete playerModel_;
	delete player_;
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// 3Dモデルの描画
	Model::PreDraw(dxCommon->GetCommandList());
	// 自キャラの描画
	player_->Draw();
	Model::PostDraw();
}

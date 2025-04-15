#include "GameScene.h"
using namespace KamataEngine;

void GameScene::Initialize() {
#pragma region 自キャラの初期化
	playerTecstureHandle_ = TextureManager::Load("mario.jpg");
	playerSprite_ = Sprite::Create(playerTecstureHandle_, {100, 50});
	// 3Dモデルの作成
	playerModel_ = Model::Create();
	// ワールドトランスフォームの初期化
	playerWorldTransform_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize();

#pragma endregion
	// カメラの初期化
	camera_.Initialize();
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
	// 自キャラの描画
	player_->Draw();
}

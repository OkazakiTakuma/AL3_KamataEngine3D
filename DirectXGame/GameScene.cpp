#include "GameScene.h"
using namespace KamataEngine;
// 横幅
void GameScene::Initialize() {
#pragma region 画像・3Dモデル生成
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
#pragma region スカイドームとマップチップの初期化
	// スカイドームの生成
	skydome_ = new Skydome();
	skydome_->Initialize(&debugCamera_->GetCamera());
	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	// マップチップデータの読み込み
	mapChipField_->LoadMapChipCsv("Resources/stage.csv");
	GenerateBlock();
#pragma endregion

#pragma region 自キャラの初期化
	tecstureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(tecstureHandle_, {100, 50});
	// 座標をマップチップ番号で指定
	playerPosition_ = mapChipField_->GetMapChipPositionByIndex(2, 17);
	// プレイヤーの初期座標を設定
	playerPosition_.x *= kBlockWidth;
	playerPosition_.y *= kBlockHeight;
	playerPosition_.z = -0.01f;
	// 自キャラの生成
	// プレイヤーモデルの生成
	playerModel_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(playerPosition_, playerModel_);
	player_->SetMapChipField(mapChipField_);
#pragma endregion
#pragma region 敵キャラの初期化
	// 敵キャラのモデルの生成
	enemyModel_ = Model::CreateFromOBJ("Enemy", true);
	// 敵キャラの初期座標を設定
	Vector3 enemyPosition2D = mapChipField_->GetMapChipPositionByIndex(5, 16);
	enemyPosition2D.x *= kBlockWidth;
	enemyPosition2D.y *= kBlockHeight;
	KamataEngine::Vector3 enemyPosition = enemyPosition2D;
	// 敵キャラの生成
	for (int i = 0; i < 5; i++) {
		Enemy* enemy = new Enemy();
		// 敵キャラの初期化
		enemy->Initialize(enemyPosition, enemyModel_);
		// 敵キャラを敵リストに追加
		enemies_.push_back(enemy);
		// 敵キャラの初期座標をずらす
		enemyPosition.x += 2.0f;
	}

#pragma endregion

#pragma endregion
#pragma region 死んだときのパーティクル
	// 死んだときのパーティクルの初期化
	// 死んだときのパーティクルの生成
	deathParticles_ = new DeathParticles();
	deathParticlesModel_ = Model::CreateFromOBJ("deathParticle", true);

	// カメラの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea(Rect(25, 100, 15, 100));
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete skydome_;
	delete debugCamera_;
	delete mapChipField_;
	delete player_;
	delete playerModel_;

	delete cameraController_;
	delete deathParticles_;
	delete deathParticlesModel_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete enemyModel_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransFormBlocks_.clear();
}

void GameScene::Update() {
	switch (phase_) {
	case GameScene::Phase::kPlay:
		// 自キャラの更新
		skydome_->Update();
		player_->Update();
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		cameraController_->Update();
		// 敵キャラの更新

#pragma region ブロック配置の更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}

				// ワールドトランスフォームの更新
				WorldTransformUpdate(*worldTransformBlock);
			}
		}
#pragma endregion
		CheckALLCollision();

		break;
	case GameScene::Phase::kDeath:
		skydome_->Update();

		// 死んだときのパーティクルの更新
		if (deathParticles_) {
			deathParticles_->Update();
		}
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		cameraController_->Update();
		// 敵キャラの更新

#pragma region ブロック配置の更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}

				// ワールドトランスフォームの更新
				WorldTransformUpdate(*worldTransformBlock);
			}
		}
#pragma endregion
		break;
	default:
		break;
	}
	ChangePhase();
#ifdef _DEBUG
	// デバッグカメラの更新
	debugCamera_->Update();

#endif
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dモデルの描画
	Model::PreDraw(dxCommon->GetCommandList());
	// スカイドームの描画
	skydome_->Draw();
	// スプライトの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			model_->Draw(*worldTransformBlock, cameraController_->GetCamera());
		}
	}
	// 自キャラの描画
	if (!player_->IsDead()) {
		player_->Draw(&cameraController_->GetCamera());
	}
	// 敵キャラの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw(&cameraController_->GetCamera());
	}
	// 死んだときのパーティクルの描画
	if (phase_ == Phase::kDeath) {
		deathParticles_->Draw(&cameraController_->GetCamera());
	}
#ifdef _DEBUG
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 10}, {1.0f, 0.0f, 0.0f, 1.0f});
#endif

	Model::PostDraw();
}

void GameScene::GenerateBlock() {
#pragma region ブロック配置の初期化
	// 要素数
	const uint32_t kNumBlockVertical = 20;
	const uint32_t kNumBlockHorizontal = 100;

	// 要素数の変更
	worldTransFormBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		worldTransFormBlocks_[i].resize(kNumBlockHorizontal);
	}
	// 生成
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeIndex(j, i) == MapChipType::kBlock) {
				// ワールドトランスフォームの生成
				WorldTransform* worldTransForm = new WorldTransform();
				worldTransForm->Initialize();
				worldTransFormBlocks_[i][j] = worldTransForm;
				worldTransFormBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				worldTransFormBlocks_[i][j]->translation_.x *= kBlockWidth;
				worldTransFormBlocks_[i][j]->translation_.y *= kBlockHeight;
			}
		}
	}
	scale_ = {0};
	rotate_ = {0};
	translate_ = {0};
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case GameScene::Phase::kPlay:
		if (player_->IsDead()) {

			phase_ = Phase::kDeath;
			const Vector3& deathParticlePosition = player_->GetWorldPosition();
			deathParticles_->Initialize(deathParticlePosition, deathParticlesModel_);
		}
		break;
	case GameScene::Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		break;
	default:
		break;
	}
}

void GameScene::CheckALLCollision() {
#pragma region 自キャラと敵キャラとの衝突判定
	// 自キャラと敵キャラの衝突判定
	AABB playerAABB = player_->GetAABB();
	AABB enemyAABB;
	for (Enemy* enemy : enemies_) {
		enemyAABB = enemy->GetAABB();
		if (IsCollisionAABBToAABB(playerAABB, enemyAABB)) {
			// 衝突した場合の処理
			player_->OnCollisionEnemy(enemy);
			enemy->OnCollisionPlayer(player_);
		}
	}
#pragma endregion
}

#include "GameScene.h"
// #include "PrimitiveDrawer.h"

using namespace KamataEngine;

// AL302_13の27ページから再開

void GameScene::Initialize() {
	// 初期化処理

	textureHandle_ = TextureManager::Load("/cube/cube.jpg");

	playerTextureHandle_ = TextureManager::Load("/sample.png");

	enemyTextureHandle_ = TextureManager::Load("/uvChecker.png");

	//// スプライトインスタンスの生成
	// sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// 3Dモデルの生成
	playerModel_ = Model::CreateFromOBJ("player", true);
	// 敵
	enemyModel_ = Model::CreateFromOBJ("enemy", true);
	// ぱーてぃくる
	modelParticle_ = Model::CreateFromOBJ("deathParticle", true);
	// あたっく
	attackModel_ = Model::CreateFromOBJ("attak", true);

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.farZ = 1000.0f; // カメラの奥行きの最大値を設定
	camera_.Initialize();

	// サウンドデータの読み込み
	// soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");

	// 音声再生
	// Audio::GetInstance()->PlayWave(soundDataHandle_);

	// voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);

	// ライン描画が参照するカメラを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());

	modelBlock_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// 天球の生成
	skydome_ = new Skydome();
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/stage.csv");
	GenerateBlocks();

	//=================
	// プレイヤー
	//=================

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	KamataEngine::Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 6);

	// 座標をマップチップ番号で指定
	player_->Initialize(playerModel_, playerTextureHandle_, attackModel_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	cameraController_ = new CameraController();
	// CameraControllerの初期化
	cameraController_->Initialize(&camera_); // 自前のCameraを使うなら引数なしでOK

	// プレイヤーを追従対象としてセット
	cameraController_->SetTarget(player_);

	// カメラ位置を即時合わせる
	cameraController_->Reset();

	// 仮の生成処理。後で消す
	deathParticles_ = new DethParticles;
	// 死亡パーティクルの初期化
	deathParticles_->Initialize(modelParticle_, playerTextureHandle_, &camera_, playerPosition);

	//=======
	// 敵
	//=======
	for (int32_t i = 0; i < kEnemyNum; i++) {

		Enemy* newEnemy = new Enemy();

		// 敵の初期化
		KamataEngine::Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(8 + (i * 2), 18);
		newEnemy->Initialize(enemyModel_, &camera_, enemyPosition);
		newEnemy->SetGameScene(this); // 🔴 GameSceneを渡すのを忘れずに！
		enemies_.push_back(newEnemy);
	}

	isDethParticlesActive_ = true;

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, duration_);

	phase_ = Phase::kFadeIn;

	// ヒットエフェクト用のモデル読み込み
	hitEffectModel_ = Model::CreateFromOBJ("hit", true);
	HitEffect::SetModel(hitEffectModel_);
	// ヒットエフェクト用のカメラ設定
	HitEffect::SetCamera(&camera_);
}

GameScene::~GameScene() {
	delete sprite_;
	delete playerModel_;
	delete enemyModel_;
	delete debugCamera_;
	delete player_;
	delete modelBlock_;
	delete modelSkydome_;
	delete skydome_;
	delete cameraController_;
	delete mapChipField_;
	delete hitEffectModel_;
	hitEffects_.clear();
	enemies_.clear();
	// ワールドトランスフォームの解放
	for (std::vector<KamataEngine::WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// 敵の解放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}

	delete deathParticles_;
	delete fade_;
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	// ブロック1個分の横幅

	// 要素数を変更する
	// 列数を設定(縦方向のブロック数)
	worldTransformBlocks_.resize(numBlockVirtical); // 縦方向
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		// 1列の要素数を設定(横方向のブロック数)
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipField::MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

// 全ての当たり判定を行う
void GameScene::CheckAllCollisions() {
#pragma region プレイヤーと敵の当たり判定

	AABB aabb1, aabb2;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵全ての当たり判定
	for (Enemy* enemy : enemies_) {
		if (enemy->IsCollisionDisabled())
			continue; // 当たり判定が無効な敵はスキップ

		if (!enemy)
			continue; // nullptrチェック
		// 敵の座標
		aabb2 = enemy->GetAABB();
		// 当たり判定
		if (aabb_.IsCollisionAABB(aabb1, aabb2)) {
			// 当たったときの処理
			player_->OnCollision(enemy);
			// 敵の当たり判定
			enemy->OnCollision(player_);
		}
	}

#pragma endregion

#pragma region プレイヤーとアイテムの当たり判定

#pragma endregion

#pragma region 自分の弾と敵の当たり判定

#pragma endregion
}

void GameScene::CreateHitEffect(KamataEngine::Vector3& spawnPosition) {
	// ヒットエフェクトの生成
	HitEffect* newHitEffect = HitEffect::Create(spawnPosition);

	hitEffects_.push_back(newHitEffect);
}

void GameScene::Update() {
	// 更新処理

	// switch (phase_) {
	//	case Phase::kPlay:
	//		// ゲームプレイ中の処理
	//
	//		//// スプライトの今の座標を取得
	//		// Vector2 position = sprite_->GetPosition();
	//		//// 座標を{2,1}移動
	//		// position.x += 2.0f;
	//		// position.y += 1.0f;
	//
	//		// 移動した座標をスプライトに反映
	//		// sprite_->SetPosition(position);
	//
	//		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//			// 音声再生
	//			//	Audio::GetInstance()->StopWave(soundDataHandle_);
	//		}
	//
	//// デモウィンドウの表示を有効化
	// #ifdef _DEBUG
	//		ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
	// #endif
	//
	//		debugCamera_->Update();
	//
	//		player_->Updata();
	//
	//		if (player_->IsDead()) {
	//			// 死亡演出フェーズに切り替え
	//			phase_ = Phase::kDeath;
	//			// 死亡パーティクルの位置をプレイヤーの位置に設定
	//			deathParticles_->Initialize(modelParticle_, playerTextureHandle_, &camera_, player_->GetPosition());
	//		}
	//
	//		// ポインタがnullではないときだけ行う
	//		//	if (enemy_!=nullptr) {
	//
	//		for (Enemy* enemy : enemies_) {
	//			if (enemy) {
	//				enemy->Update();
	//			}
	//		}
	//
	//		//}
	//		// ブロックの更新
	//		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	//			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
	//				if (!worldTransformBlock)
	//					continue;
	//				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
	//				// 定数バッファを転送する
	//				worldTransformBlock->TransferMatrix();
	//			}
	//		}
	//
	// #ifdef _DEBUG
	//		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	//
	//			isDebugCameraActive_ = true;
	//		}
	// #endif
	//
	//		CheckAllCollisions();
	//
	//		if (isDebugCameraActive_) {
	//			// デバッグカメラの更新
	//			camera_.matView = debugCamera_->GetCamera().matView;
	//			camera_.matProjection = debugCamera_->GetCamera().matProjection;
	//			// ビュープロジェクション行列の更新と転送
	//			camera_.TransferMatrix();
	//		} else {
	//
	//			// ビュープロジェクション行列の更新と転送
	//			camera_.UpdateMatrix();
	//		}
	//
	//		// 天球の処理
	//		skydome_->Update();
	//
	//		cameraController_->Update();
	//
	//		break;
	//
	//	case Phase::kDeath:
	//
	//		// 天球の処理
	//		skydome_->Update();
	//
	//		for (Enemy* enemy : enemies_) {
	//			if (enemy) {
	//				enemy->Update();
	//			}
	//		}
	//
	//		if (isDebugCameraActive_) {
	//			// デバッグカメラの更新
	//			camera_.matView = debugCamera_->GetCamera().matView;
	//			camera_.matProjection = debugCamera_->GetCamera().matProjection;
	//			// ビュープロジェクション行列の更新と転送
	//			camera_.TransferMatrix();
	//		} else {
	//
	//			// ビュープロジェクション行列の更新と転送
	//			camera_.UpdateMatrix();
	//		}
	//
	//		if (isDethParticlesActive_) {
	//			deathParticles_->Updata();
	//		}
	//
	//		// ブロックの更新
	//		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
	//			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
	//				if (!worldTransformBlock)
	//					continue;
	//				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
	//				// 定数バッファを転送する
	//				worldTransformBlock->TransferMatrix();
	//			}
	//		}
	//		if (deathParticles_ && deathParticles_->IsFinished()) {
	//			finished_ = true; // シーンを終了するフラグを立てる
	//		}
	//		break;
	//	}

	fade_->Update();

	switch (phase_) {
	case Phase::kFadeIn:

		// ▼ kPlay 相当の処理を実行
		debugCamera_->Update();
		player_->Update();

		cameraController_->Update();
		skydome_->Update();

		// ブロックの更新なども忘れずに
		for (auto& row : worldTransformBlocks_) {
			for (auto* block : row) {
				if (!block)
					continue;
				block->matWorld_ = MakeAffineMatrix(block->scale_, block->rotation_, block->translation_);
				block->TransferMatrix();
			}
		}

		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}
		break;

	case Phase::kPlay:
		//	fade_->Update();
		// ゲームプレイ中の処理

		//// スプライトの今の座標を取得
		// Vector2 position = sprite_->GetPosition();
		//// 座標を{2,1}移動
		// position.x += 2.0f;
		// position.y += 1.0f;

		// 移動した座標をスプライトに反映
		// sprite_->SetPosition(position);

		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// 音声再生
			//	Audio::GetInstance()->StopWave(soundDataHandle_);
		}

		// デモウィンドウの表示を有効化
#ifdef _DEBUG
		ImGui::Text("Kamata Tarou %d.%d.%d", 2050, 12, 31);
#endif

		debugCamera_->Update();

		player_->Update();

		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 死亡パーティクルの位置をプレイヤーの位置に設定
			deathParticles_->Initialize(modelParticle_, playerTextureHandle_, &camera_, player_->GetPosition());
		}

		// ポインタがnullではないときだけ行う
		//	if (enemy_!=nullptr) {

		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Update();
			}
		}

		enemies_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				delete enemy; // メモリ解放
				return true;  // リストから削除
			}
			return false; // 削除しない
		});

		for (HitEffect* hitEffect : hitEffects_) {
			if (hitEffect) {
				hitEffect->Update();
			}
		}

		//}
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファを転送する
				worldTransformBlock->TransferMatrix();
			}
		}

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

			isDebugCameraActive_ = true;
		}
#endif

		CheckAllCollisions();

		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の更新と転送
			camera_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// 天球の処理
		skydome_->Update();

		cameraController_->Update();

		break;

	case Phase::kDeath:
		//	fade_->Update();
		// 天球の処理
		skydome_->Update();

		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Update();
			}
		}

		for (HitEffect* hitEffect : hitEffects_) {
			if (hitEffect) {
				hitEffect->Update();
			}
		}

		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の更新と転送
			camera_.TransferMatrix();
		} else {

			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		if (isDethParticlesActive_) {
			deathParticles_->Updata();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファを転送する
				worldTransformBlock->TransferMatrix();
			}
		}
		if (deathParticles_ && deathParticles_->IsFinished()) {

			fade_->Start(Fade::Status::FadeOut, duration_); // 🔧 フェード開始を追加
			phase_ = Phase::kFadeOut;                       // フェードアウトフェーズに切り替え
		}
		break;

	case Phase::kFadeOut:
		//	fade_->Update();
		// 天球の処理
		skydome_->Update();

		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Update();
			}
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファを転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void GameScene::Draw() {
	// 描画処理

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	switch (phase_) {
	case Phase::kPlay:
		if (!modelParticle_) {
			OutputDebugStringA("modelParticle_ is nullptr!\n");
		}

		// スプライト描画前処理
		Sprite::PreDraw(dxCommon->GetCommandList());

		// スプライト描画後処理
		Sprite::PostDraw();
		// model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

		//// 3Dモデル描画
		// model_->Draw(worldTransform_, camera_, textureHandle_);

		// 3Dモデルの描画前処理
		Model::PreDraw(dxCommon->GetCommandList());

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, camera_, textureHandle_);
			}
		}

		// 自キャラの描画
		player_->Draw();
	
		// 敵の描画
		// if (enemy_ != nullptr) {
		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Draw();
			}
		}

		//}
		// === Skydome描画（背景） ===
		skydome_->Draw();

		if (isDethParticlesActive_) {
			deathParticles_->Draw();
		}

		for (HitEffect* hitEffect : hitEffects_) {

			hitEffect->Draw();
		}

		// 3Dモデルの描画後処理
		Model::PostDraw();

		break;
	case Phase::kDeath:

		if (!modelParticle_) {
			OutputDebugStringA("modelParticle_ is nullptr!\n");
		}

		// スプライト描画前処理
		Sprite::PreDraw(dxCommon->GetCommandList());

		// スプライト描画後処理
		Sprite::PostDraw();

		// sprite_->Draw();

		// 3Dモデルの描画前処理
		Model::PreDraw(dxCommon->GetCommandList());

		// model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

		//// 3Dモデル描画
		// model_->Draw(worldTransform_, camera_, textureHandle_);

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, camera_, textureHandle_);
			}
		}

		// 敵の描画
		// if (enemy_ != nullptr) {
		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Draw();
			}
		}

		for (HitEffect* hitEffect : hitEffects_) {

			hitEffect->Draw();
		}

		//}
		// === Skydome描画（背景） ===
		skydome_->Draw();

		if (isDethParticlesActive_) {
			deathParticles_->Draw();
		}

		// 3Dモデルの描画後処理
		Model::PostDraw();
		break;
	case Phase::kFadeIn:

		// 3Dモデルの描画前処理
		Model::PreDraw(dxCommon->GetCommandList());

		if (!modelParticle_) {
			OutputDebugStringA("modelParticle_ is nullptr!\n");
		}

		// model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);

		//// 3Dモデル描画
		// model_->Draw(worldTransform_, camera_, textureHandle_);

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, camera_, textureHandle_);
			}
		}
		// 自キャラの描画
		player_->Draw();

		// 敵の描画
		// if (enemy_ != nullptr) {
		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Draw();
			}
		}

		//}
		// === Skydome描画（背景） ===
		skydome_->Draw();

		if (isDethParticlesActive_) {
			deathParticles_->Draw();
		}

		// 3Dモデルの描画後処理
		Model::PostDraw();

		// 3Dモデルの描画後処理
		Model::PostDraw();
		break;

	case Phase::kFadeOut:

		// 3Dモデルの描画前処理
		Model::PreDraw(dxCommon->GetCommandList());
		// === Skydome描画（背景） ===
		skydome_->Draw();

		// ブロックの描画
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				modelBlock_->Draw(*worldTransformBlock, camera_, textureHandle_);
			}
		}

		// 敵の描画
		// if (enemy_ != nullptr) {
		for (Enemy* enemy : enemies_) {
			if (enemy) {
				enemy->Draw();
			}
		}

		// 3Dモデルの描画後処理
		Model::PostDraw();
		break;
	}

	fade_->Draw();

	// ラインを描画
	// PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});
}

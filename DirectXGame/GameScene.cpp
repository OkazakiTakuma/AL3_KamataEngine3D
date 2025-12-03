#include "GameScene.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <algorithm>
#include <utility>

using namespace KamataEngine;

void GameScene::Initialize() {
	// 3Dモデル生成と初期化
	blockModel_ = Model::CreateFromOBJ("cube", true);
	ladderModel_ = Model::CreateFromOBJ("ladder", true);
	worldTransform_.Initialize();

	// カメラとデバッグカメラ初期化
	camera_.Initialize();
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());

	// スカイドームとマップ読み込み
	skydome_ = new Skydome();
	skydome_->Initialize(&debugCamera_->GetCamera());

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/stage.csv");
	GenerateBlock();

	// プレイヤー初期化
	tecstureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(tecstureHandle_, {100, 50});

	playerPosition_ = mapChipField_->GetMapChipPositionByIndex(2, 17);
	playerPosition_.x *= kBlockWidth;
	playerPosition_.y *= kBlockHeight;
	playerPosition_.z = -0.01f;

	playerModel_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	player_->Initialize(playerPosition_, playerModel_);
	player_->SetMapChipField(mapChipField_);

	// 敵の生成（同じモデルを共有）
	enemyModel_ = Model::CreateFromOBJ("Enemy", true);
	Vector3 enemyPosition2D = mapChipField_->GetMapChipPositionByIndex(5, 16);
	enemyPosition2D.x *= kBlockWidth;
	enemyPosition2D.y *= kBlockHeight;
	KamataEngine::Vector3 enemyPosition = enemyPosition2D;
	for (int i = 0; i < 5; ++i) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(enemyPosition, enemyModel_);
		enemies_.push_back(enemy);
		enemyPosition.x += 2.0f;
	}

	// 死亡パーティクル
	deathParticles_ = new DeathParticles();
	deathParticlesModel_ = Model::CreateFromOBJ("deathParticle", true);

	// カメラコントローラ
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea(Rect(0, 100, 10, 100));

	// フェード開始
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::FadeIn, 1.0f);
	phase_ = Phase::kFadeIn;
}

GameScene::~GameScene() {
	delete sprite_;
	delete blockModel_;
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
	for (auto& line : worldTransFormBlocks_) {
		for (WorldTransform* wt : line) {
			delete wt;
		}
	}
	worldTransFormBlocks_.clear();

	delete fade_;
}

void GameScene::Update() {
	// フェーズごとの更新
	switch (phase_) {
	case Phase::kFadeIn:
		cameraController_->Update();
		skydome_->Update();
		fade_->Update();
		UpdateBlocksTransforms();
		if (fade_->IsFinished())
			phase_ = Phase::kPlay;
		break;

	case Phase::kPlay:
		skydome_->Update();

		UpdateEnemies();
		cameraController_->Update();
		UpdateBlocksTransforms();
		CheckALLCollision();
		player_->Update();
		player_->SetMapChipField(mapChipField_);

		if (player_) {
			// プレイヤーの座標からインデックス取得
			MapChipField::IndexSet idx = mapChipField_->GetMapChipIndexByPosition(player_->GetWorldPosition());
			// ブロックタイプ判定
			if (mapChipField_->GetMapChipTypeIndex(idx.xIndex, idx.yIndex) == MapChipType::kCrackBlock) {
				// 既にタイマーが登録されているかチェック
				auto it = std::find_if(crackBlockTimers.begin(), crackBlockTimers.end(),
					[&](const CrackBlockTimer& t) { return t.xIndex == idx.xIndex && t.yIndex == idx.yIndex; });
				if (it == crackBlockTimers.end()) {
					// 新規登録
					crackBlockTimers.push_back({ idx.xIndex, idx.yIndex, 0.0f });
				}
			}
			// タイマー更新
			for (auto& timer : crackBlockTimers) {
				timer.timer += 1.0f / 60.0f; // 1フレーム=1/60秒
			}
			// 3秒経過したブロックをBlankに変更
			for (auto it = crackBlockTimers.begin(); it != crackBlockTimers.end();) {
				if (it->timer >= 3.0f) {

					// ブロックタイプ変更
					if (mapChipField_->GetMapChipTypeIndex(it->xIndex, it->yIndex) == MapChipType::kCrackBlock) {
						mapChipField_->SetMapChipTypeIndex(it->xIndex, it->yIndex, MapChipType::kBlank);
					}
					it = crackBlockTimers.erase(it);
				} else {
					++it;
				}
			}
		}
		break;

	case Phase::kDeath:
		skydome_->Update();
		if (deathParticles_)
			deathParticles_->Update();
		UpdateEnemies();
		cameraController_->Update();
		UpdateBlocksTransforms();
		break;

	case Phase::kFadeOut:
		skydome_->Update();
		fade_->Update();
		UpdateBlocksTransforms();
		if (fade_->IsFinished()) {
			finished_ = true;
			return;
		}
		break;

	default:
		break;
	}

	ChangePhase();

#ifdef _DEBUG
	debugCamera_->Update();
#endif
}

void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// スカイドームは先に描画
	if (skydome_)
		skydome_->Draw();

	// ブロックの描画
	for (uint32_t i = 0; i < worldTransFormBlocks_.size(); ++i) {
		for (uint32_t j = 0; j < worldTransFormBlocks_[i].size(); ++j) {
			WorldTransform* wt = worldTransFormBlocks_[i][j];
			if (!wt) continue;

			MapChipType type = mapChipField_->GetMapChipTypeIndex(j, i);
			if (type == MapChipType::kBlock || type == MapChipType::kCrackBlock) {
				blockModel_->Draw(*wt, cameraController_->GetCamera());
			} else if (type == MapChipType::kLadder) {
				ladderModel_->Draw(*wt, cameraController_->GetCamera());
			} else if (type == MapChipType::kIceBlocK) {
				blockModel_->Draw(*wt, cameraController_->GetCamera(),0,nullptr);
			}
				
			
		}
	}

	// プレイヤー描画（生きているときのみ）
	if (player_ && !player_->IsDead()) {
		player_->Draw(&cameraController_->GetCamera());
	}

	// 敵描画
	for (Enemy* enemy : enemies_) {
		if (enemy && !enemy->IsDead()) {

			enemy->Draw(&cameraController_->GetCamera());
		}
	}

	// 死亡時パーティクル
	if (phase_ == Phase::kDeath && deathParticles_) {
		deathParticles_->Draw(&cameraController_->GetCamera());
	}

	// フェード処理（フェード中はモデル描画後にフェードを描画して終了）
	if (phase_ == Phase::kFadeIn || phase_ == Phase::kFadeOut) {
		fade_->Draw();
		Model::PostDraw();
		return;
	}

#ifdef _DEBUG
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {10, 0, 10}, {1.0f, 0.0f, 0.0f, 1.0f});
#endif

	Model::PostDraw();
}

void GameScene::GenerateBlock() {
	const uint32_t kNumBlockVertical = 20;
	const uint32_t kNumBlockHorizontal = 100;

	worldTransFormBlocks_.assign(kNumBlockVertical, std::vector<WorldTransform*>(kNumBlockHorizontal, nullptr));

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* wt = new WorldTransform();
				wt->Initialize();
				wt->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				wt->translation_.x *= kBlockWidth;
				wt->translation_.y *= kBlockHeight;
				wt->scale_ = {0.5f, 0.5f, 0.5f};
				worldTransFormBlocks_[i][j] = wt;

			} else if (mapChipField_->GetMapChipTypeIndex(j, i) == MapChipType::kLadder) {
				WorldTransform* wt = new WorldTransform();
				wt->Initialize();
				wt->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				wt->translation_.x *= kBlockWidth;
				wt->translation_.y *= kBlockHeight;
				wt->scale_ = {1.0f, 1.0f, 1.0f};
				worldTransFormBlocks_[i][j] = wt;
			} else if (mapChipField_->GetMapChipTypeIndex(j, i) == MapChipType::kCrackBlock) {
				WorldTransform* wt = new WorldTransform();
				wt->Initialize();
				wt->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				wt->translation_.x *= kBlockWidth;
				wt->translation_.y *= kBlockHeight;
				wt->scale_ = {0.5f, 0.5f, 0.5f};
				worldTransFormBlocks_[i][j] = wt;
			} else if (mapChipField_->GetMapChipTypeIndex(j, i) == MapChipType::kIceBlocK) {
				WorldTransform* wt = new WorldTransform();
				wt->Initialize();
				wt->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				wt->translation_.x *= kBlockWidth;
				wt->translation_.y *= kBlockHeight;
				wt->scale_ = {0.5f, 0.5f, 0.5f};
				worldTransFormBlocks_[i][j] = wt;
			}
		}
	}
	scale_ = {0};
	rotate_ = {0};
	translate_ = {0};
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_ && player_->IsDead()) {
			phase_ = Phase::kDeath;
			const Vector3& deathPos = player_->GetWorldPosition();
			if (deathParticles_)
				deathParticles_->Initialize(deathPos, deathParticlesModel_);
		}
		break;

	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			fade_->Start(Fade::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;

	default:
		break;
	}
}

void GameScene::CheckALLCollision() {
	// 自キャラと敵のAABB衝突
	if (!player_)
		return;

	AABB playerAABB = player_->GetAABB();
	for (Enemy* enemy : enemies_) {
		if (!enemy)
			continue;
		AABB enemyAABB = enemy->GetAABB();
		if (IsCollisionAABBToAABB(playerAABB, enemyAABB)) {
			player_->OnCollisionEnemy(enemy);
			enemy->OnCollisionPlayer(player_);
		}
	}

	// 攻撃範囲判定（プレイヤーが攻撃状態のとき）
	if (player_->GetIsAttack()) {
		const int maxRange = static_cast<int>(player_->GetMaxAttackRange());
		Vector3 playerPos = player_->GetWorldPosition();
		playerPos.y *= 2.0f;

		bool hitAny = false;
		for (int r = 0; r < maxRange; ++r) {
			AABB attackRange;
			float rf = static_cast<float>(r);
			if (player_->GetLRDirection() == LRDirection::kLeft) {
				// 左向き

				attackRange.min = playerPos - Vector3(rf, 0, kWidth / 2.0f);
				attackRange.max = playerPos + Vector3(0, rf, kWidth / 2.0f);
			} else {
				// 右向き
				attackRange.min = playerPos + Vector3(0, 0, -kWidth / 2.0f);
				attackRange.max = playerPos + Vector3(rf, rf, kWidth / 2.0f);
			
			}

			for (Enemy* enemy : enemies_) {
				if (!enemy)
					continue;
				if (IsCollisionAABBToAABB(attackRange, enemy->GetAABB())) {
					// ヒットした敵をターゲットに設定し、Player 側でヒット処理（ライン表示）を行う
					player_->SetTargetWorldPosition(enemy->GetWorldPosition());
					player_->SetIsAttackHit(true);
					enemy->OnCollisionPlayer(player_);
					player_->SetIsAttack(false); // 攻撃フラグ解除
					hitAny = true;
					break;
				}
			}
			// 1体でもヒットしたらループを抜ける
			if (hitAny) {
				break;
			}
			// 攻撃がヒットしなかった場合、攻撃フラグを解除
			player_->SetIsAttackHit(false);

		}
	}
}

void GameScene::UpdateBlocksTransforms() {
	for (auto& line : worldTransFormBlocks_) {
		for (WorldTransform* wt : line) {
			if (!wt)
				continue;
			WorldTransformUpdate(*wt);
		}
	}
}

void GameScene::UpdateEnemies() {
	for (Enemy* enemy : enemies_) {
		if (enemy) {
			enemy->Update();
		}
	}
	// 死亡した敵をリストから削除してメモリ解放
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false; // 返り値がない場合C4715が発生するため、falseを返す
	});
}
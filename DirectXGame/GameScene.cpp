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
	playerPosition_ = mapChipField_->GetMapChipPositionByIndex(2, 16);
	// プレイヤーの初期座標を設定
	playerPosition_.x *= kBlockWidth;
	playerPosition_.y *= kBlockHeight;
	playerPosition_.z = -0.01f;
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(playerPosition_);
	player_->SetMapChipField(mapChipField_);
#pragma endregion
#pragma endregion

	// カメラの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();
	cameraController_->SetMovableArea(Rect(25, 100,15, 100));

}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete skydome_;
	delete debugCamera_;
	delete mapChipField_;
	delete player_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}


	}
	worldTransFormBlocks_.clear();
}

void GameScene::Update() {
	// 自キャラの更新
	cameraController_->Update();
	player_->Update();
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
	player_->Draw(&cameraController_->GetCamera());
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
			if (mapChipField_->GetMapChipTypeIndex(j,i)==MapChipType::kBlock) {
				// ワールドトランスフォームの生成
				WorldTransform* worldTransForm = new WorldTransform();
				worldTransForm->Initialize();
				worldTransFormBlocks_[i][j]=worldTransForm;
				worldTransFormBlocks_[i][j]->translation_=mapChipField_->GetMapChipPositionByIndex(j,i);
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

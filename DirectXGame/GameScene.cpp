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

	// スカイドームの生成
	skydome_ = new Skydome();
	skydome_->Initialize(&debugCamera_->GetCamera());
	// マップチップフィールドの生成
	mapChipField_ = new MapChipFIeld;
	// マップチップデータの読み込み
	mapChipField_->LoadMapChipCsv("Resources/stage.csv");
	GenerateBlock();
#pragma endregion


}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete skydome_;
	delete debugCamera_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}


	}
	worldTransFormBlocks_.clear();
}

void GameScene::Update() {
#pragma region ブロック配置の更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransFormBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}



			// ワールドトランスフォームの更新
			// scaleの変換
			scale_.x = static_cast<float>(worldTransformBlock->scale_.x);
			scale_.y = static_cast<float>(worldTransformBlock->scale_.y);
			scale_.z = static_cast<float>(worldTransformBlock->scale_.z);
			// rotateの変換
			rotate_.x = static_cast<float>(worldTransformBlock->rotation_.x);
			rotate_.y = static_cast<float>(worldTransformBlock->rotation_.y);
			rotate_.z = static_cast<float>(worldTransformBlock->rotation_.z);
			// translateの変換
			translate_.x = static_cast<float>(worldTransformBlock->translation_.x);
			translate_.y = static_cast<float>(worldTransformBlock->translation_.y);
			translate_.z = static_cast<float>(worldTransformBlock->translation_.z);
			// アフィン変換
			Matrix4x4Afifne affine = MakeAffineMatrix(scale_, rotate_, translate_);
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					// ワールドトランスフォームの行列にアフィン変換を適用
					worldTransformBlock->matWorld_.m[j][i] = affine.m[j][i];
				}
			}
			// ワールドトランスフォームの転送
			worldTransformBlock->TransferMatrix();
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
			model_->Draw(*worldTransformBlock, debugCamera_->GetCamera());
		}
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

	// 横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

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

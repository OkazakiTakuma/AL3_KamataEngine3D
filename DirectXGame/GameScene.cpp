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
#pragma endregion

#pragma region ブロック配置の初期化
	// 要素数
	const uint32_t kNumBlockVertical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	// 横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数の変更
	worldTransformBlocks_.resize(kNumBlockVertical);

	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// 生成
	for (uint32_t i = 0; i < kNumBlockVertical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (i % 2 == 0 || j % 2 == 0) {
				// ワールドトランスフォームの生成
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
				if (i % 2 == 0 && j % 2 == 0) {
					// ワールドトランスフォームの生成
					worldTransformBlocks_[i][j] = nullptr;
				}
			}
		}
	}
	scale_ = {0};
	rotate_ = {0};
	translate_ = {0};
#pragma endregion
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete skydome_;
	delete debugCamera_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Update() {
#pragma region ブロック配置の更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
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
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
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

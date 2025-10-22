#include "TitleScene.h"

void TitleScene::Initialize() {
	// 初期化処理
	// ここではタイトルシーンの初期化を行います。
	// 例えば、背景画像の読み込みやタイトルテキストの設定などを行います。


	titleModel_ = KamataEngine::Model::CreateFromOBJ("title", true);
	playerModel_ = KamataEngine::Model::CreateFromOBJ("player", true);
	
	playerTextureHandle_ = KamataEngine::TextureManager::Load("sample.png"); // プレイヤー用のテクスチャハンドル

	titlePosition_ = {-14.0f, 12.0f, 0.0f}; // タイトルの位置

	attackModel_ = KamataEngine::Model::CreateFromOBJ("attak", true); // 攻撃用のモデル

	// カメラの生成（←これが重要！）
	camera_ = new KamataEngine::Camera();
	camera_->Initialize(); // ← これが必須！

	worldTransform_.Initialize();

	title_ = new Title();

	title_->Initialize(titleModel_,  camera_, titlePosition_);

	player_ = new Player();
	player_->Initialize(playerModel_, playerTextureHandle_ ,attackModel_,camera_, playerPosition_);

	fade_ = new Fade();
	fade_->Initialize(); // フェードの初期化
	fade_->Start(Fade::Status::FadeIn, duration_);

}

TitleScene::~TitleScene() {
	// デストラクタでリソースを解放
	delete title_;
	delete player_;
	delete camera_; // カメラの解放
	if (titleModel_) {
		delete titleModel_;
	}
	if (playerModel_) {
		delete playerModel_;
	}
	delete fade_; // フェードの解放
}

void TitleScene::Update() {
	fade_->Update(); // フェードの更新処理

	switch (phase_) {
	case Phase::kFadeIn:
		title_->Update();
		player_->TitleUpdata();
		if (fade_->IsFinished()) {
			// フェードイン終了判定
			phase_ = Phase::kMain;
		}
		break;

	case Phase::kMain:
		title_->Update();
		player_->TitleUpdata();

		// スペースキーを押したらフェードアウト開始
		if (KamataEngine::Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, duration_);
			phase_ = Phase::kFadeOut;
		}
		break;

	case Phase::kFadeOut:
		title_->Update();
		player_->TitleUpdata();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void TitleScene::Draw() {

  KamataEngine::DirectXCommon* dxCommon = KamataEngine::DirectXCommon::GetInstance();

	KamataEngine::Model::PreDraw(dxCommon->GetCommandList());

	// タイトルモデルの描画
	if (title_) {
		title_->Draw();
	}

	
		player_->Draw(); // プレイヤーの描画処理
	

	KamataEngine::Model::PostDraw();

	fade_->Draw(); // フェードの描画処理


}
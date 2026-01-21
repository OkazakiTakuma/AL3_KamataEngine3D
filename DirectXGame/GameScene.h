#pragma once
#include "KamataEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include <vector>
#include "MapChipFiled.h"
#include "CameraController.h"
#include "Enemy.h"
#include "AABB.h"
#include "DethParticles.h"
#include "Fade.h"
#include "HitEffect.h"

enum class Phase { 
	kFadeIn, 
	kPlay, 
	kDeath,
	kFadeOut
};



// ゲームシーン
class GameScene {

public:


	// 初期化
ain
	void Initialize();

	// デストラクタ
	~GameScene();

	// 更新
	void Update();

	// 描画
	void Draw();


	void GenerateBlocks();

	//全ての当たり判定を行う
	void CheckAllCollisions();

	void ChangePhase();

	void CreateHitEffect(KamataEngine::Vector3& spawnPosition);


	// スプライト
	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Sprite* playerSprite_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Model* enemyModel_ = nullptr;
	KamataEngine::Model* attackModel_ = nullptr; 
	// ブロックモデル
	KamataEngine::Model* modelBlock_ = nullptr;
	//パーティクル
	KamataEngine::Model* modelParticle_ = nullptr;
	//ヒットエフェクト用のモデル
	KamataEngine::Model* hitEffectModel_ = nullptr;

	//テクスチャハンドル
	uint32_t dethTextureHandle_ = 0; // テクスチャハンドル

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera camera_;

	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;


	Player* player_ = nullptr;
	//リスト
	std::list<Enemy*> enemies_;
	std::list<HitEffect*> hitEffects_;


	Skydome* skydome_ = nullptr;



	CameraController* cameraController_ = nullptr;

	MapChipField* mapChipField_ ;

	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	KamataEngine::Matrix4x4 matrix_;

	KamataEngine::Sprite* maskSprite_ = nullptr;

	KamataEngine::Sprite* blackSprite_ = nullptr;

	KamataEngine::Model* modelSkydome_ = nullptr;

	DethParticles* deathParticles_ = nullptr;

	AABB aabb_;

	Phase phase_;

	bool IsFinished() const { return finished_; } // シーンが終了したかどうかを返す

	Fade* fade_ = nullptr;

private:
	uint32_t textureHandle_ = 0; // テクスチャハンドル

	uint32_t playerTextureHandle_ = 0; // プレイヤーテクスチャハンドル

	uint32_t enemyTextureHandle_ = 0; // 敵のテクスチャハンドル

	bool finished_ = false; // シーンが終了したかどうかを示すフラグ

	uint32_t soundDataHandle_ = 0; // サウンドデータハンドル

	uint32_t voiceHandle_ = 0; // 音声ハンドル

	float inputFloat3[3] = {0.0f, 0.0f, 0.0f}; // ImGui用のfloat配列

	// Matrix4x4 viewMatrix;

	bool isDethParticlesActive_ ; // 死亡パーティクルのアクティブ状態

	bool isDebugCameraActive_ = false;

	float duration_ = 1.0f;

	static inline const int kEnemyNum = 3; // 敵の数

};


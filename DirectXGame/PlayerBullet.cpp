#include "PlayerBullet.h"

using namespace KamataEngine;

Model* PlayerBullet::model_ = nullptr;

void PlayerBullet::LoadModel() {
	model_ = Model::CreateFromOBJ("player"); // ← あなたの弾モデル名に合わせて変更
}

void PlayerBullet::Initialize(const Vector3& position,const Vector3& velosity) { 
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");
	//引数で受け取った速度をメンバ変数に代入
	velosity_ = velosity;


	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	SetCollisionAttribute(kCollisionAttributePlayerBullet);

	// 自分は「敵と敵弾」に当たりたい場合（例）
	SetCollisionMask(kCollisionAttributeEnemy | kCollisionAttributeEnemyBullet);

}




void PlayerBullet::OnCollision() {

isDead_ = true; // 衝突したらデスフラグを立てる

}

void PlayerBullet::Updata() {

	if (--deathTimer_ <= 0) {
		isDead_=true;
	}

	//座標を移動させる
	worldTransform_.translation_ += velosity_;

	WorldTrnasformUpdate(worldTransform_);

}


void PlayerBullet::Draw(const Camera& camera) {

	model_->Draw(worldTransform_, camera, textureHandle_);

}
#pragma once
#include "KamataEngine.h"



class Fade {

	

	public:

		// フェードの状態
	    enum class Status {
		    None,    // フェードなし
		    FadeIn,  // フェードイン
		    FadeOut, // フェードアウト
	    };

	void Initialize();

	void Update();

	void Draw();

	void Start(Status status, float duration);

	void Stop();

	KamataEngine::Sprite* sprite_ = nullptr; // スプライトのポインタ

	uint32_t textureHandle_ = 0u; // テクスチャハンドル

	bool IsFinished() const; // フェードが終了したかどうかを返す

	private:

	Status status_ = Status::None; // フェードの状態
		
	float duration_ = 0.0f;
	//経過時間カウンター
	float counter_ = 0.0f; // フェードの経過時間カウンター

};

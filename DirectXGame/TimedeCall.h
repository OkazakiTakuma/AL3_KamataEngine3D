#pragma once
#include <functional>

/// <summary>
/// 時限発動クラス
/// </summary>
class TimedeCall {
public:
	// コンストラクタ
	TimedeCall(std::function<void()> callback, uint32_t time);

	// デストラクタ
	~TimedeCall() = default;

	// 更新
	void Update();

	// 終了したかどうか
	bool IsFinished() const { return isCalled_; }

private:
	std::function<void()> callback_;
	uint32_t time_;
	bool isCalled_ = false;
};

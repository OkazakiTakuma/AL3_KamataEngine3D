#include "TimedeCall.h"

TimedeCall::TimedeCall(std::function<void()> callback, uint32_t time) : callback_(callback), time_(time) {}

void TimedeCall::Update() {
	if (isCalled_)
		return;

	if (time_ > 0) {
		--time_;
	}

	if (time_ == 0 && !isCalled_) {
		isCalled_ = true;
		callback_();
	}
}

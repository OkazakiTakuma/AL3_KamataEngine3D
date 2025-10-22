#pragma once
#include <random>

class Random {
public:
	Random() : mt_(rd_()) {}

	int GetInt(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(mt_);
	}

	float GetFloat(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt_);
	}

private:
	std::random_device rd_;
	std::mt19937 mt_;
};
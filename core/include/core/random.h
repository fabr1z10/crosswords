#pragma once
#include <random>
#include <cstdint>

class Random {
public:
	/// Random seed (non-deterministic)
	Random();

	/// Fixed seed (deterministic)
	explicit Random(std::uint32_t seed);

	/// Inclusive range [min, max]
	int nextInt(int min, int max);

private:
	std::mt19937 _engine;
};



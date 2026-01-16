#include "core/random.h"
#include <random>

Random::Random()
{
	std::random_device rd;
	_engine.seed(rd());   // non-deterministic seed
}

Random::Random(std::uint32_t seed)
		: _engine(seed)       // deterministic seed
{
}

int Random::nextInt(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(_engine);
}
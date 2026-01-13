#include "core/random.h"
#include <random>

// Define random engine and device globally
unsigned int seed=12345;
std::random_device rd;
std::mt19937 gen(seed); // Use Mersenne Twister engine

int getRandomNumber(int max) {
    std::uniform_int_distribution<> dist(0, max);
    return dist(gen);
}

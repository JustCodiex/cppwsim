#include "Random.h"
#include <random>
#include <ctime>

Random::Random() {
	m_seed = (int)clock();
	srand(m_seed);
}

Random::Random(int seed) {
	m_seed = seed;
	srand(m_seed);
}

int Random::NextInt(int minInclusive, int maxExclusive) {
	return (rand() % (maxExclusive - minInclusive)) + minInclusive;
}

float Random::NextPercentage() {
	return NextInt(0, 10001) / 10000.0f;
}

bool Random::NextBool(float chanceTrue) {
	return true;
}

void Random::Reset() {
	srand(m_seed);
}

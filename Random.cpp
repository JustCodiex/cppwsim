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
	
	if (minInclusive > maxExclusive) {
		int buffer = minInclusive;
		minInclusive = maxExclusive;
		maxExclusive = buffer;
	} else if (minInclusive == maxExclusive) {
		return minInclusive;
	}

	return minInclusive + (int)((maxExclusive - minInclusive) * (rand() / (RAND_MAX + 1.0)));
	//return (rand() % (maxExclusive - minInclusive)) + minInclusive;
}

float Random::NextFloat(float max) {
	return max * NextPercentage();
}

float Random::NextFloat(float min, float max) {
	return NextFloat(max - min) + min;
}

float Random::NextPercentage() {
	return NextInt(0, 10001) / 10000.0f;
}

bool Random::NextBool(float chanceTrue) {
	float p = NextPercentage();
	return p > (1.0f - chanceTrue);
}

void Random::Reset() {
	srand(m_seed);
}

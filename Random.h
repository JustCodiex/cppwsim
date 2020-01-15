#pragma once

class Random {

public:

	Random();
	Random(int seed);

	int NextInt(int minInclusive, int maxExclusive);

	float NextPercentage();

	bool NextBool(float chanceTrue);

	void Reset();

private:

	int m_seed;

};

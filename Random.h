#pragma once
#include <vector>

template<typename T>
struct Weight {
	float w;
	T val;
	Weight() { w = 0.0f; }
	Weight(float weight, T value) {
		w = weight;
		val = value;
	}
};

class Random {

public:

	Random();
	Random(int seed);

	int NextInt(int minInclusive, int maxExclusive);
	float NextFloat(float max);
	float NextFloat(float min, float max);

	float NextPercentage();

	bool NextBool(float chanceTrue);

	void Reset();

	template<typename T>
	T Select(std::vector<Weight<T>> values) {
		float weightSum = 0.0f;
		for (int i = 0; i < values.size(); i++) {
			Weight<T> e = values[i];
			weightSum += e.w;
		}
		float rnd = NextFloat(weightSum);
		for (int i = 0; i < values.size(); i++) {
			Weight<T> e = values[i];
			if (rnd < e.w)
				return e.val;
			rnd -= e.w;
		}
		return T();
	}

private:

	int m_seed;

};

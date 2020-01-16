#pragma once
#include <vector>
#include "Random.h"
#include "Legislature.h"
#include "Government.h"
#include "CityDistrict.h"

class City {

public:
	enum class Size {
		CITYSIZE_SMALL,
		CITYSIZE_LARGE,
		CITYSIZE_METROPOLIS,
	};

public:

	City();
	City(Size size, Random random);

	unsigned int GetPopulationSize() { return m_population; }

private:

	std::vector<CityDistrict> m_districts;
	unsigned int m_population;

};

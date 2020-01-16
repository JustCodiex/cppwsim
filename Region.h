#pragma once
#include <vector>
#include "City.h"
#include "Random.h"
#include "Legislature.h"

class Region {

public:

	Region();

	void Generate(bool isPartOfFederation, Random random);

	unsigned int GetPopulationSize();

private:

	void GenerateCities(Random random);

private:

	std::vector<City*> m_cities;

	Legislature* m_regionalCouncil;

};

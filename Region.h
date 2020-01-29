#pragma once
#include <vector>
#include "Random.h"
#include "Legislature.h"
#include "CountryAdministrationLevel.h"

class Region : public CountryAdministrationLevel {

public:

	Region();

	void Generate(bool isPartOfFederation, Random random);

	unsigned int GetPopulationSize() override;
	int GetCityCount();

	std::vector<City*> GetCities() override { return m_cities; }

	int GetAdminLevel() override { return 1; }

private:

	void GenerateCities(Random random);

private:

	std::vector<City*> m_cities;

	Legislature* m_regionalCouncil;

};

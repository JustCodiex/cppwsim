#pragma once
#include "stdlib.h"
#include "Legislature.h"
#include "CountryAdministrationLevel.h"

class Region : public CountryAdministrationLevel {

public:

	Region();

	void Generate(bool isPartOfFederation, Random random);

	PopSize GetPopulationSize() override;
	int GetCityCount();

	std::vector<City*> GetCities() override { return m_cities; }

	int GetAdminLevel() override { return 1; }

	void UpdateEconomy();
	void UpdateDemographics();

	City* GetCityByName(std::string name);

private:

	City* NewCity(int size, Random random);

	void GenerateCities(Random random);

private:

	std::vector<City*> m_cities;

	Legislature* m_regionalCouncil;

};

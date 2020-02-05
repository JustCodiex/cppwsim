#pragma once
#include "stdlib.h"
#include "Legislature.h"
#include "CountryAdministrationLevel.h"

class State;

class Region : public CountryAdministrationLevel {

public:

	Region();

	void Generate(bool isPartOfFederation, Random random);

	PopSize GetPopulationSize() override;
	int GetCityCount();

	std::vector<City*> GetCities() override { return m_cities; }

	int GetAdminLevel() override { return 1; }

	void UpdateEconomy(int days);
	void UpdateDemographics();

	City* GetCityByName(std::string name);

	State* GetState() { return m_state; }
	void SetState(State* pState) { m_state = pState; }

private:

	City* NewCity(int size, Random random);

	void GenerateCities(Random random);

private:

	State* m_state;
	std::vector<City*> m_cities;

	Legislature* m_regionalCouncil;

};

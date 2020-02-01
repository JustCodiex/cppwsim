#pragma once
#include "stdlib.h"
#include "Region.h"
#include "Government.h"
#include "Legislature.h"

class State : public CountryAdministrationLevel {

public:
	
	State();
	State(bool isMain);

	void GenerateRegions(Random random);

	PopSize GetPopulationSize() override;
	int GetCityCount();

	std::vector<Region*> GetRegions() { return m_regions; }

	std::vector<City*> GetCities() override;

	int GetAdminLevel() override { return 2; }

	void UpdateEconomy();
	void UpdateDemographics();

private:

	Government* m_stateGovernment;
	Legislature* m_stateLegislature;

	bool m_isMain;
	std::vector<Region*> m_regions;

};

#pragma once
#include <vector>
#include "Region.h"
#include "Random.h"
#include "Government.h"
#include "Legislature.h"

class State : public CountryAdministrationLevel {

public:
	
	State();
	State(bool isMain);

	void GenerateRegions(Random random);

	unsigned int GetPopulationSize() override;
	int GetCityCount();

	std::vector<Region*> GetRegions() { return m_regions; }

	std::vector<City*> GetCities() override;

	int GetAdminLevel() override { return 1; }

private:

	Government* m_stateGovernment;
	Legislature* m_stateLegislature;

	bool m_isMain;
	std::vector<Region*> m_regions;

};

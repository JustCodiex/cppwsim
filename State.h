#pragma once
#include <vector>
#include "Region.h"
#include "Random.h"
#include "Government.h"
#include "Legislature.h"

class State {

public:
	
	State();
	State(bool isMain);

	void GenerateRegions(Random random);

	unsigned int GetPopulationSize();
	int GetCityCount();

	std::vector<Region*> GetRegions() { return m_regions; }

private:

	Government* m_stateGovernment;
	Legislature* m_stateLegislature;

	bool m_isMain;
	std::vector<Region*> m_regions;

};

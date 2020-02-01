#include "State.h"

State::State() {
	m_stateGovernment = 0;
	m_stateLegislature = 0;
	m_isMain = true;
}

State::State(bool isMain) {
	m_isMain = isMain;
	if (isMain) {
		m_stateGovernment = 0;
		m_stateLegislature = 0;
	} else {
		m_stateGovernment = new Government;
		m_stateLegislature = new Legislature;
	}
}

void State::GenerateRegions(Random random) {

	int maxRegion = 10;
	
	if (m_isMain) {
		maxRegion += 31;
	}

	int regionCount = random.NextInt(1, 10);

	for (int i = 0; i < regionCount; i++) {

		Region* region = new Region();
		region->Generate(!m_isMain, random);

		m_regions.push_back(region);

	}

}

PopSize State::GetPopulationSize() {
	PopSize count = 0;
	for (auto region : m_regions) {
		count += region->GetPopulationSize();
	}
	return count;
}

int State::GetCityCount() {
	int count = 0;
	for (auto region : m_regions) {
		region += region->GetCityCount();
	}
	return count;
}

std::vector<City*> State::GetCities() {

	std::vector<City*> cities;

	for (auto region : m_regions) {
		for (auto city : region->GetCities()) {
			cities.push_back(city);
		}
	}

	return cities;

}

void State::UpdateEconomy() {

	for (auto region : m_regions) {
		region->UpdateEconomy();
	}

}

void State::UpdateDemographics() {

	for (auto region : m_regions) {
		region->UpdateDemographics();
	}

}

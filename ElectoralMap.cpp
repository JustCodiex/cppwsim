#include "ElectoralMap.h"
#include "Country.h"

#include <algorithm>

ElectoralMap::ElectoralMap() {
	m_electoralLevel = ElectionLevel::National;
}

ElectoralMap::ElectoralMap(ElectionLevel lvl) {
	m_electoralLevel = lvl;
}

void ElectoralMap::CreateNationalElectoralDistrict(Country* pCountry, unsigned short seats) {

	// Clear district
	this->ClearElectoralDistricts();

	// Get cities
	std::vector<City*> cities = pCountry->GetCities();

	// Create districts from cities
	CreateDistrictsFromCities(pCountry, cities, seats);

}

void ElectoralMap::CreateDistrictsFromCities(Country* pCountry, std::vector<City*> cities, unsigned short targetCount) {

	// For all cities in country
	for (size_t i = 0; i < cities.size(); i++) {

		// Create new district
		ElectoralDistrict* pDistrict = new ElectoralDistrict(ElectionLevel::National);
		pDistrict->SetDistrict(cities[i], 1.0f, pCountry->GetProfile());

		// Add district
		m_electoralDistricts.push_back(pDistrict);

	}

	// Handle cases where there are more or less seats than cities
	if (cities.size() < targetCount) { // We split large cities

		// As long as we have less than the districts we need
		while (m_electoralDistricts.size() < targetCount) {

			// Index of largest city
			int largest = 0;

			// For all city districts
			for (size_t i = 1; i < m_electoralDistricts.size(); i++) {

				PopSize thisPop = m_electoralDistricts[i]->GetVoterCount(); // Current population we're looking at
				PopSize largestPop = m_electoralDistricts[largest]->GetVoterCount(); // Currently largest population

				// Is this city larger than found largest?
				if (thisPop > largestPop) {
					largest = (int)i; // Update
				}
			}

			// Splt largest district
			ElectoralDistrict* pNewDistrict = m_electoralDistricts[largest]->Split(pCountry->GetProfile());

			// Add new district
			m_electoralDistricts.push_back(pNewDistrict);

		}

	} else if (cities.size() > targetCount) { // We merge small cities

		// As long as we have more districts than we need
		while (m_electoralDistricts.size() > targetCount) {

			// Index of smallest city
			size_t smallest = 0;

			// Index of next-smallest city
			size_t nextSmallest = 1;

			// For all cities
			for (size_t i = 0; i < m_electoralDistricts.size(); i++) {

				// Get voters in district
				PopSize voters = m_electoralDistricts[i]->GetVoterCount();

				// If v[smallest] < v[i] < v[nextSmallest] then update nextsmallest
				if (voters > m_electoralDistricts[smallest]->GetVoterCount() && voters < m_electoralDistricts[nextSmallest]->GetVoterCount()) {
					nextSmallest = i;
				} else if (voters < m_electoralDistricts[smallest]->GetVoterCount()) { // else if smaller than smallest

					// Update next smallest
					nextSmallest = smallest;

					// Udpate smallest
					smallest = i;

				}

			}

			// Get pointers
			ElectoralDistrict* pSmallestDistrict = m_electoralDistricts[smallest];
			ElectoralDistrict* pNextSmallestDistrict = m_electoralDistricts[nextSmallest];

			// Merge districts
			ElectoralDistrict* pNewDistrict = ElectoralDistrict::Merge(pSmallestDistrict, pNextSmallestDistrict, pCountry->GetProfile());

			// Remove smallest from electoral list
			m_electoralDistricts.erase(m_electoralDistricts.begin() + smallest);

			// Remove next smallest (and we might have to subtract index by one if greater than smallest.
			if (nextSmallest > smallest) {
				m_electoralDistricts.erase(m_electoralDistricts.begin() + (nextSmallest - (size_t)1));
			} else {
				m_electoralDistricts.erase(m_electoralDistricts.begin() + nextSmallest);
			}

			// Delete the smallest district
			if (pSmallestDistrict) {
				delete pSmallestDistrict;
			}

			// Delete the next smallest district
			if (pNextSmallestDistrict) {
				delete pNextSmallestDistrict;
			}

			// Add merged districts
			m_electoralDistricts.push_back(pNewDistrict);

		}

	}

}

void ElectoralMap::CreateNationalElectoralCollege(Country* pCountry, unsigned short electors, bool byRegion, bool favourSmall) {

	// Clear our working space
	this->ClearElectoralDistricts();

	// Remember how many districts we need
	unsigned short districts = electors;

	// Should we favour large regions/states => distribute electors based on population
	if (!favourSmall) {

		// Administrative levels (regions or states)
		std::vector<CountryAdministrationLevel*> adminLvls;

		// The total population size
		PopSize totalPop = 0;

		// Populate vector
		if (byRegion) {
			for (auto reg : pCountry->GetRegions()) {
				adminLvls.push_back(reg);
				totalPop += reg->GetPopulationSize();
			}
		} else {
			for (auto state : pCountry->GetStates()) {
				adminLvls.push_back(state);
				totalPop += state->GetPopulationSize();
			}
		}

		// Sort from size
		std::sort(adminLvls.begin(), adminLvls.end(), [](CountryAdministrationLevel* a, CountryAdministrationLevel* b) -> bool { return a->GetPopulationSize() > b->GetPopulationSize(); });

		// For each administrative level
		for (auto admin : adminLvls) {
		
			// Calculate elector count
			unsigned short count = (unsigned short)(admin->GetPopulationSize() / (double)totalPop);

			// Get cities
			std::vector<City*> cities = admin->GetCities();

			// Create districts from cities
			CreateDistrictsFromCities(pCountry, cities, count);

		}

	} else {

		// We distribute electors equally => favours small states/regions
		unsigned short districtsPerArea = districts / ((byRegion) ? pCountry->GetRegionCount() : pCountry->GetStateCount());

		// Administrative levels (regions or states)
		std::vector<CountryAdministrationLevel*> adminLvls;

		// Populate vector
		if (byRegion) {
			for (auto reg : pCountry->GetRegions()) {
				adminLvls.push_back(reg);
			}
		} else {
			for (auto state : pCountry->GetStates()) {
				adminLvls.push_back(state);
			}
		}

		// For each administrative level
		for (auto admin : adminLvls) {

			// Get cities
			std::vector<City*> cities = admin->GetCities();

			// Create districts from cities
			CreateDistrictsFromCities(pCountry, cities, districtsPerArea);

		}

	}

}

void ElectoralMap::ClearElectoralDistricts() {

	// Loop through all the districts
	for (size_t i = 0; i < m_electoralDistricts.size(); i++) {

		// Make sure it's valid
		if (m_electoralDistricts[i]) {

			// Delete it from memory
			delete m_electoralDistricts[i];

		}

	}

	// Clear
	m_electoralDistricts.clear();

}

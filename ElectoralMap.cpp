#include "ElectoralMap.h"
#include "Country.h"

ElectoralMap::ElectoralMap() {
	m_electoralLevel = ElectionLevel::National;
}

ElectoralMap::ElectoralMap(ElectionLevel lvl) {
	m_electoralLevel = lvl;
}

void ElectoralMap::CreateNationalElectoralDistrict(Country* pCountry, unsigned short seats) {

	// Clear district
	this->ClearElectoralDistricts();

	// District count is the same as seat count (1:1 ratio)
	int districtCount = seats;

	// Get cities
	std::vector<City*> cities = pCountry->GetCities();

	// For all cities in country
	for (size_t i = 0; i < cities.size(); i++) {

		// Create new district
		ElectoralDistrict* pDistrict = new ElectoralDistrict(ElectionLevel::National);
		pDistrict->SetDistrict(cities[i], 1.0f, pCountry->GetProfile());

		// Add district
		m_electoralDistricts.push_back(pDistrict);

	}

	// Handle cases where there are more or less seats than cities
	if (cities.size() < districtCount) { // We split large cities

		// As long as we have less than the districts we need
		while (m_electoralDistricts.size() < districtCount) {

			// Index of largest city
			int largest = 0;

			// For all city districts
			for (size_t i = 1; i < m_electoralDistricts.size(); i++) {

				unsigned int thisPop = m_electoralDistricts[i]->GetVoterCount();
				unsigned int largestPop = m_electoralDistricts[largest]->GetVoterCount();

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

	} else if (cities.size() > districtCount) { // We merge small cities

		// As long as we have more districts than we need
		while (m_electoralDistricts.size() > districtCount) {

			// Index of smallest city
			size_t smallest = 0;

			// Index of next-smallest city
			size_t nextSmallest = 1;

			// For all cities
			for (size_t i = 0; i < m_electoralDistricts.size(); i++) {

				// Get voters in district
				unsigned int voters = m_electoralDistricts[i]->GetVoterCount();

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
			ElectoralDistrict* pNewDistrict = ElectoralDistrict::MergeAndDelete(pSmallestDistrict, pNextSmallestDistrict, pCountry->GetProfile());

			// Remove smallest from electoral list
			m_electoralDistricts.erase(m_electoralDistricts.begin() + smallest);

			// Remove next smallest (and we might have to subtract index by one if greater than smallest.
			if (nextSmallest > smallest) {
				m_electoralDistricts.erase(m_electoralDistricts.begin() + (nextSmallest - (size_t)1));
			} else {
				m_electoralDistricts.erase(m_electoralDistricts.begin() + nextSmallest);
			}

			if (pSmallestDistrict) {
				delete pSmallestDistrict;
			}

			if (pNextSmallestDistrict) {
				delete pNextSmallestDistrict;
			}

			// Add merged districts
			m_electoralDistricts.push_back(pNewDistrict);

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

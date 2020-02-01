#pragma once
#include "stdlib.h"
#include "ElectoralDistrict.h"

class Country;

class ElectoralMap {

public:

	ElectoralMap();
	ElectoralMap(ElectionLevel lvl);

	void ClearElectoralDistricts();

	void CreateNationalElectoralDistrict(Country* pCountry, unsigned short seats);
	void CreateNationalElectoralCollege(Country* pCountry, unsigned short electors, bool byRegion, bool favourSmall);

	ElectoralDistrict* GetElectoralDistrictByIndex(unsigned int index) { return m_electoralDistricts[index]; }

	size_t GetElectoralDistrictCount() { return m_electoralDistricts.size(); }

private:

	// The actual algorithm of splitting or merging districts
	void CreateDistrictsFromCities(Country* pCountry, std::vector<City*> cities, unsigned short targetCount);

private:

	ElectionLevel m_electoralLevel;
	std::vector<ElectoralDistrict*> m_electoralDistricts;

};

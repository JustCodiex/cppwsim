#pragma once
#include <vector>
#include "ElectoralDistrict.h"

class Country;

class ElectoralMap {

public:

	ElectoralMap();
	ElectoralMap(ElectionLevel lvl);

	void ClearElectoralDistricts();

	void CreateNationalElectoralDistrict(Country* pCountry, unsigned short seats);

	ElectoralDistrict* GetElectoralDistrictByIndex(unsigned int index) { return m_electoralDistricts[index]; }

	size_t GetElectoralDistrictCount() { return m_electoralDistricts.size(); }

private:

	ElectionLevel m_electoralLevel;
	std::vector<ElectoralDistrict*> m_electoralDistricts;

};

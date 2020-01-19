#include "ElectoralDistrict.h"
#include "Country.h"
#include "Ballot.h"
#include "City.h"

ElectoralDistrict::ElectoralDistrict(ElectionLevel lvl) {
	m_electionLevel = lvl;
}

void ElectoralDistrict::SetDistrict(City* pCity, float fSizePercentage) {
	ElectoralCity city;
	city.city = pCity;
	city.partition = fSizePercentage;
	m_cities.push_back(city);
}

unsigned int ElectoralDistrict::GetVoterCount() {
	unsigned int voters = 0;
	for (auto city : m_cities) {
		voters += (unsigned int)(city.city->GetPopulationSize() * city.partition);
	}
	return voters; // TODO: Make better
}

ElectoralDistrict* ElectoralDistrict::Split() {

	ElectoralDistrict* pNewDistrict = new ElectoralDistrict(m_electionLevel);
	
	if (this->m_cities.size() == 1) {
		pNewDistrict->SetDistrict(m_cities[0].city, m_cities[0].partition / 2.0f);
		m_cities[0].partition /= 2.0f;
	} else {
		pNewDistrict->m_cities.push_back(m_cities[0]);
		m_cities.erase(m_cities.begin());
	}

	return pNewDistrict;

}

ElectoralDistrict* ElectoralDistrict::MergeAndDelete(ElectoralDistrict* pSmall, ElectoralDistrict* pAlmostSmall) {

	ElectoralDistrict* pMergedDistrict = new ElectoralDistrict(pSmall->m_electionLevel);
	
	for (auto smallCity : pSmall->m_cities) {
		pMergedDistrict->m_cities.push_back(smallCity);
	}

	for (auto smallCity : pAlmostSmall->m_cities) {
		pMergedDistrict->m_cities.push_back(smallCity);
	}

	delete pSmall;
	delete pAlmostSmall;

	return pMergedDistrict;

}

Ballot* ElectoralDistrict::CreateBallot(Country* pCountry) {

	Ballot* pBallot = new Ballot;

	return pBallot;

}

ElectoralDistrictResult ElectoralDistrict::CastVotes(Ballot* pBallot, Country* pCountry) {

	ElectoralDistrictResult results;

	return results;

}

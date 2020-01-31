#include "ElectoralDistrict.h"
#include "Election.h"
#include "Country.h"
#include "Ballot.h"
#include "City.h"

ElectoralDistrict::ElectoralDistrict(ElectionLevel lvl) {
	m_electionLevel = lvl;
	m_voters = 0;
	m_seat = 0;
}

void ElectoralDistrict::SetDistrict(City* pCity, float fSizePercentage, CountryProfile* pProfile) {
	ElectoralCity city;
	city.city = pCity;
	city.partition = fSizePercentage;
	m_cities.push_back(city);
	UpdateVoterCount(pProfile);
}

void ElectoralDistrict::UpdateVoterCount(CountryProfile* pProfile) {
	m_voters = 0;
	for (auto city : m_cities) {
		m_voters += (PopSize)(city.city->GetVoterCount(pProfile) * (double)city.partition);
	}
}

ElectoralDistrict* ElectoralDistrict::Split(CountryProfile* pProfile) {

	ElectoralDistrict* pNewDistrict = new ElectoralDistrict(m_electionLevel);
	
	if (this->m_cities.size() == 1) {
		pNewDistrict->SetDistrict(m_cities[0].city, m_cities[0].partition / 2.0f, pProfile);
		m_cities[0].partition = m_cities[0].partition / 2.0f;
	} else {
		pNewDistrict->m_cities.push_back(m_cities[0]);
		m_cities.erase(m_cities.begin());
	}

	this->UpdateVoterCount(pProfile);
	pNewDistrict->UpdateVoterCount(pProfile);

	return pNewDistrict;

}

ElectoralDistrict* ElectoralDistrict::Merge(ElectoralDistrict* pSmall, ElectoralDistrict* pAlmostSmall, CountryProfile* pProfile) {

	ElectoralDistrict* pMergedDistrict = new ElectoralDistrict(pSmall->m_electionLevel);
	
	for (auto smallCity : pSmall->m_cities) {
		pMergedDistrict->m_cities.push_back(smallCity);
	}

	for (auto smallCity : pAlmostSmall->m_cities) {
		pMergedDistrict->m_cities.push_back(smallCity);
	}

	pMergedDistrict->UpdateVoterCount(pProfile);

	return pMergedDistrict;

}

Ballot* ElectoralDistrict::CreateBallot(Country* pCountry) {

	// Create ballot
	Ballot* pBallot = new Ballot;

	// Get a list of all parties in the country
	std::vector<Country::Party> parties = pCountry->GetPartyList();

	// Loop through all the parties
	for (size_t i = 0; i < parties.size(); i++) {

		// Get the party at index
		Country::Party p = parties[i];

		// Make sure the election level is appropriate
		if (p.level >= m_electionLevel) {

			// Get the candidate (either generate one or retrieve one)
			Politician* candidate = p.party->GetCandidate((int)this->m_electionLevel, this->m_cities[0].city);

			// Make sure the candidate is valid
			if (candidate) {

				// Add candidate to ballot
				pBallot->AddCandidate(candidate);

			}

		}

	}

	// Return ballot
	return pBallot;

}

ElectoralDistrictResult ElectoralDistrict::CastVotes(Ballot* pBallot, Country* pCountry) {

	ElectoralDistrictResult results;
	results.totalVotes = 0;

	if (m_electionLevel == ElectionLevel::National) {

		std::map<Politician*, PopSize> result = Election::National(pBallot, this, pCountry);

		for (auto votes : result) {
			results.votes[votes.first] = votes.second;
			results.totalVotes += results.votes[votes.first];
		}

		results.turnout = (results.totalVotes / (double)m_voters);

		for (auto votes : result) {
			results.voteshare[votes.first] = results.votes[votes.first] / (double)results.totalVotes;
		}

	}

	return results;

}

#include "PoliticalParty.h"
#include "names_party.h"
#include "Politician.h"
#include "Country.h"

PoliticalParty::PoliticalParty() {
	
	m_colour = PartyColour::BLACK;
	m_ideology = Ideology(POLITICAL_IDEOLOGY::ID_AGRAGIANISM);
	m_establishYear = 1800;
	m_targetCountry = 0;
	m_breakoutFrom = 0;
	m_random = Random();

}

void PoliticalParty::CreateParty(int establishYear, POLITICAL_IDEOLOGY ideology, Country* pCountry, Random random) {

	// Assign target country
	m_targetCountry = pCountry;

	// Create the ideology
	m_ideology = Ideology(ideology);

	// Get party name
	m_name = FindPartyName(pCountry, random);

	// Generate some stances
	m_ideology.RegenerateStancesFromIndexedIdeology(random);

	// Get establishment year
	m_establishYear = establishYear - random.NextInt(0, 31);

	// Convert the name to a short variant
	ConvertNameToShort();

}

void PoliticalParty::CreateParty(int establishYear, Country* pCountry, Random random) {

	// Get a random, indexed, ideology
	POLITICAL_IDEOLOGY ideology = GetRandomIdeology(establishYear, random);

	// Call the updated CreateParty method
	this->CreateParty(establishYear, ideology, pCountry, random);

}

std::string PoliticalParty::FindPartyName(Country* pCountry, Random random) {

	// store result here
	std::string result;

	// flag for checking if country has party name
	bool hasPartyName = false;

	// Get party name
	while (result == "" || (hasPartyName = pCountry->HasParty(result))) {

		// Has party name => can we modify it?
		if (hasPartyName && random.NextBool(0.1f)) {

			auto breakFrom = pCountry->GetPartyByName(result);

			if (random.NextBool(0.5f)) {

				if (random.NextBool(0.5f)) {
					result = "New " + result;
				} else {
					result = "Renwers of " + result;
				}

			} else {

				if (random.NextBool(0.5f)) {
					result = result + " Seperatists";
				} else {
					result = result + " Breakouts";
				}

			}

			if (breakFrom != NULL) {
				this->SetBreakout(breakFrom);
			}

		} else {

			// Get the party name
			result = GetPartyName(m_ideology.GetIdeology(), random);

		}

	}

	// Return the string result
	return result;

}

void PoliticalParty::ConvertNameToShort() {

	// Use initial party index over shortenings (AKA use party letters)
	if (m_targetCountry->m_useInitialPartyIndex) {

		// Get letter (65='A')
		char s = 65 + (char)m_targetCountry->m_partyShorts.size();

		// Assign letter
		m_short = s;

		// Inform the system this has been taken
		m_targetCountry->m_partyShorts.push_back(m_short);

	} else {



	}

}

Politician* PoliticalParty::FindCandidate(int electionLvl, City* pCity, Country* pCountry) {

	for (auto member : m_members) {

		if (member.pMember->GetSeat() == 0 && member.electionLvl >= electionLvl && member.pCity == pCity && member.pCountry == pCountry) {

			return member.pMember;

		}

	}

	return NULL;

}

Politician* PoliticalParty::NewCandidate(int electionLvl, City* pCity, Country* pCountry) {

	PartyMember member;
	member.electionLvl = electionLvl;
	member.pCity = pCity;
	member.pCountry = pCountry;

	member.pMember = new Politician(m_random);
	member.pMember->SetParty(this, m_random.NextPercentage());

	m_members.push_back(member);

	return member.pMember;

}

Politician* PoliticalParty::GetCandidate(int electionLvl, City* pCity) {

	Politician* pCandidate = FindCandidate(electionLvl, pCity, m_targetCountry);
	if (pCandidate) {
		return pCandidate;
	}

	return NewCandidate(electionLvl, pCity, m_targetCountry);

}

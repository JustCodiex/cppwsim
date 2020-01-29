#include "PoliticalParty.h"
#include "names_party.h"
#include "Politician.h"
#include "Country.h"

#include <sstream>

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
	ConvertNameToShort(pCountry);

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

void PoliticalParty::ConvertNameToShort(Country* pCountry) {

	// Use initial party index over shortenings (AKA use party letters)
	if (m_targetCountry->m_useInitialPartyIndex) {

		// Get letter (65='A')
		char s = 65 + (char)m_targetCountry->m_partyShorts.size();

		// Assign letter
		m_short = s;

		// Inform the system this has been taken
		m_targetCountry->m_partyShorts.push_back(m_short);

	} else {

		// Split the string
		std::vector<std::string> words = split_string(m_name, ' ');

		// Single word
		if (words.size() == 1) {

			// Are there more than 3 letters
			if (words[0].size() >= 3) {
				m_short = to_upper(words[0].substr(0, 3));
			} else {
				m_short = to_upper(words[0]);
			}

		} else if (words.size() == 2) { // Two words

			// Combine the first two letters of first word and last letter of second word
			m_short = to_upper(words[0].substr(0, 2) + words[1].substr(0, 1));

		} else if (words.size() >= 3) { // Three or more words

			// Add first letter of first three words
			std::stringstream ss;
			ss << words[0][0];
			ss << words[1][0];
			ss << words[2][0];

			// Get the uppercase of the three letters
			m_short = to_upper(ss.str());

		}

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

std::vector<Politician*> PoliticalParty::GetMinisterialCandidates() {

	std::vector<Politician*> candidates;
	std::vector<PolicyArea> policyAreasToCover;

	for (int i = 0; i < (int)PolicyArea::_COUNT_; i++)
		policyAreasToCover.push_back((PolicyArea)i);

	for (auto member : m_members) {

		auto found = std::find(policyAreasToCover.begin(), policyAreasToCover.end(), member.pMember->GetSpeciality());

		if (found != policyAreasToCover.end()) {
			candidates.push_back(member.pMember);
			policyAreasToCover.erase(found);
		}

	}

	for (PolicyArea a : policyAreasToCover) {

		Politician* candidate = this->NewCandidate(NULL, NULL, NULL);
		candidate->SetSpeciality(a);

		candidates.push_back(candidate);

	}

	return candidates;

}

void PoliticalParty::AddMember(Politician* pPolitician) {
	
	PartyMember member;
	member.electionLvl = (int)ElectionLevel::National;
	member.pCity = 0;
	member.pCountry = 0;
	member.pMember = pPolitician;

	m_members.push_back(member);

}

Politician* PoliticalParty::GetPrimeCandidate() {
	return this->GetCandidateWithspeciality(PolicyArea::State);
}

Politician* PoliticalParty::GetCandidateWithspeciality(PolicyArea area) {

	for (PartyMember member : m_members) {
		if (member.pMember->GetSpeciality() == area) {
			return member.pMember;
		}
	}

	Politician* newPrime = new Politician(Random());
	newPrime->SetParty(this, 1.0f);
	newPrime->SetSpeciality(area);

	this->AddMember(newPrime);

	return newPrime;

}

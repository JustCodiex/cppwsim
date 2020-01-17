#include "PoliticalParty.h"
#include "names_party.h"
#include "Country.h"

PoliticalParty::PoliticalParty() {
	
	m_colour = PartyColour::BLACK;
	m_ideology = Ideology(POLITICAL_IDEOLOGY::ID_AGRAGIANISM);
	m_establishYear = 1800;
	m_targetCountry = 0;

}

void PoliticalParty::CreateParty(int establishYear, Country* pCountry, Random random) {

	// Assign target country
	m_targetCountry = pCountry;

	// Get a random, indexed, ideology
	POLITICAL_IDEOLOGY ideology = GetRandomIdeology(establishYear, random);

	// Get party name
	m_name = GetPartyName(ideology, random);

	// Create the ideology
	m_ideology = Ideology(ideology);

	// Get establishment year
	m_establishYear = establishYear - random.NextInt(0, 31);

	// Convert the name to a short variant
	ConvertNameToShort();

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

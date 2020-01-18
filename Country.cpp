#include "Country.h"
#include "World.h"
#include "Person.h"
#include "RoyalFamily.h"

Country::Country(std::string name) {

	m_countryName = name;

	m_royals = 0;
	m_headOfState = 0;

}

void Country::GenerateCountry(Random random) {

	// Generate the geography of the country
	GenerateGeography(random);

	// Generate the government
	GenerateGovernment(random);

	// Generate political parties
	GeneratePoliticalParties(random);

}

void Country::GenerateGeography(Random random) {

	if (random.NextBool(0.33f)) {

		int stateCount = random.NextInt(2, 101);

		for (int i = 0; i < stateCount; i++) {

			State* subState = new State(false);
			subState->GenerateRegions(random);

			m_states.push_back(subState);

		}

	} else {

		State* mainState = new State(true);
		mainState->GenerateRegions(random);

		m_states.push_back(mainState);

	}

}

void Country::GenerateGovernment(Random random) {

	std::vector<Weight< GovernmentType>> govTypeChances = {
		Weight(30.0f, GovernmentType::ConstitutionalMonarchism),
		Weight(50.0f, GovernmentType::AbsoluteMonarchism),
		Weight(10.0f, GovernmentType::Democracy),
	};

	m_countryGovernment.SetGovernmentType(random.Select(govTypeChances));

	if (m_countryGovernment.GetGovernmentType() == GovernmentType::AbsoluteMonarchism) {

		GenerateRoyalFamily(random);

	} else if (m_countryGovernment.GetGovernmentType() == GovernmentType::ConstitutionalMonarchism) {

		GenerateRoyalFamily(random);

		GenerateLegislature(random);

	} else {

		m_headOfState = new Person(random);

		GenerateLegislature(random);

	}

}

void Country::GenerateRoyalFamily(Random random) {

	m_royals = new RoyalFamily;
	m_royals->CreateFamily(random);

	m_headOfState = m_royals->GetHeadOfFamily();

}

void Country::GenerateLegislature(Random random) {

	m_countryLegislature = new Legislature;
	m_countryLegislature->GenerateLegislature(this, random);

}

void Country::GeneratePoliticalParties(Random random) {

	m_useInitialPartyIndex = random.NextBool(0.5f);

	int politicalPartyList = random.NextInt(1, 4);

	for (int i = 0; i < politicalPartyList; i++) {

		PoliticalParty party = PoliticalParty();
		party.CreateParty(1800, this, random);

		m_parties.push_back(party);

	}

}

void Country::UpdateCountry(World* pWorld) {

	// Call Economy update
	this->UpdateEconomy(pWorld);

	// Call legislature update
	this->UpdateLegislature(pWorld);

}

void Country::UpdateEconomy(World* pWorld) {



}
void Country::UpdateLegislature(World* pWorld) {

	if (m_countryLegislature) {
		m_countryLegislature->UpdateLegislature(pWorld);
	}

}

unsigned int Country::GetPopulationSize() {
	unsigned int size = 0;
	for (State*& state : m_states) {
		size += state->GetPopulationSize();
	}
	return size;
}

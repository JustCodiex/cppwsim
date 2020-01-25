#include "Country.h"
#include "World.h"
#include "Person.h"
#include "RoyalFamily.h"
#include "Newspaper.h"

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

	// Finally, we can generate the country profile
	GenerateCountryProfile(random);

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

	m_countryGovernment.SetCountry(this);
	m_countryGovernment.SetGovernmentType(random.Select(govTypeChances));

	if (m_countryGovernment.GetGovernmentType() == GovernmentType::AbsoluteMonarchism) {

		GenerateRoyalFamily(random);

		m_countryGovernment.AppointGovernment();

	} else if (m_countryGovernment.GetGovernmentType() == GovernmentType::ConstitutionalMonarchism) {

		GenerateRoyalFamily(random);

		GenerateLegislature(random);

		m_countryGovernment.AppointGovernment();

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
	m_countryLegislature->RecalculateElectionDistricts();

}

void Country::GeneratePoliticalParties(Random random) {

	m_useInitialPartyIndex = random.NextBool(0.5f);

	int politicalPartyList = random.NextInt(2, 5);

	std::vector<POLITICAL_IDEOLOGY> ideologies;
	while(ideologies.size() < politicalPartyList) {
		POLITICAL_IDEOLOGY ideology = GetRandomIdeology(1800, random);
		if (std::find(ideologies.begin(), ideologies.end(), ideology) == ideologies.end() || ideologies.size() >= 3) {
			ideologies.push_back(ideology);
		}
	}

	for (int i = 0; i < politicalPartyList; i++) {

		Party party;
		party.party = new PoliticalParty();
		party.level = ElectionLevel::National;
		party.party->CreateParty(1800, ideologies[i], this, random);

		m_parties.push_back(party);

	}

}

void Country::GenerateCountryProfile(Random random) {

	// Generate countrys political stance
	m_countryProfile.countryCollectivismModifier = (random.NextPercentage() * 0.5f) - 0.25f;
	m_countryProfile.countryLibertarianModifier = (random.NextPercentage() * 0.5f) - 0.25f;

	// Generate country's political apathy
	m_countryProfile.countryPoliticalApathy = random.NextFloat(0.4f);

	// Generate voting stances (if it makes sense)
	if (m_countryGovernment.GetGovernmentType() == GovernmentType::AbsoluteMonarchism) {
		m_countryProfile.voting_female_suffrage = false;
		m_countryProfile.voting_require_business = true;
		m_countryProfile.voting_require_land = true;
		m_countryProfile.voting_require_land_and_business = true;
	} else {
		m_countryProfile.voting_female_suffrage = random.NextBool(0.02f);
		m_countryProfile.voting_require_land_and_business = random.NextBool(0.7f);
		if (!m_countryProfile.voting_require_land_and_business) {
			m_countryProfile.voting_require_business = random.NextBool(0.8f);
			m_countryProfile.voting_require_land = random.NextBool(0.9f);
		} else {
			m_countryProfile.voting_require_business = m_countryProfile.voting_require_land = true;
		}
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

int Country::GetCityCount() {
	int count = 0;
	for (State*& state : m_states) {
		count += state->GetCityCount();
	}
	return count;
}

City* Country::GetCityByUnsafeIndex(int index) {

	int i = index;
	for (State*& state : m_states) {
		for (Region*& region : state->GetRegions()) {
			if (i < region->GetCityCount()) {
				i -= region->GetCityCount();
				return region->GetCities()[i];
			} else {
				i -= region->GetCityCount();
			}
		}
	}

	return NULL;

}

std::vector<City*> Country::GetCities() {

	std::vector<City*> citylist;

	for (auto state : m_states) {
		for (auto region : state->GetRegions()) {
			for (auto city : region->GetCities()) {
				citylist.push_back(city);
			}
		}
	}

	return citylist;

}

bool Country::HasParty(std::string partyname) {
	for (auto party : m_parties) {
		if (party.party->GetName() == partyname) {
			return true;
		}
	}
	return false;
}

bool Country::hasPartyWithShort(std::string partyshort) {
	for (auto party : m_parties) {
		if (party.party->GetShort() == partyshort) {
			return true;
		}
	}
	return false;
}

PoliticalParty* Country::GetPartyByName(std::string partyname) {
	for (auto party : m_parties) {
		if (party.party->GetName() == partyname) {
			return party.party;
		}
	}
	return NULL;
}

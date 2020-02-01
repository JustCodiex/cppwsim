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

	// Generate political parties
	GeneratePoliticalParties(random);

	// Generate the government
	GenerateGovernment(random);

	// Finally, we can generate the country profile
	GenerateCountryProfile(random);

}

void Country::GenerateGeography(Random random) {

	if (random.NextBool(0.33f)) {

		int stateCount = random.NextInt(2, 55);

		for (int i = 0; i < stateCount; i++) {

			State* subState = new State(false);
			subState->GenerateRegions(random);
			subState->UpdateDemographics();
			subState->UpdateEconomy();

			m_states.push_back(subState);

		}

	} else {

		State* mainState = new State(true);
		mainState->GenerateRegions(random);
		mainState->UpdateDemographics();
		mainState->UpdateEconomy();

		m_states.push_back(mainState);

	}

}

void Country::GenerateGovernment(Random random) {

	std::vector<Weight< GovernmentType>> govTypeChances = {
		Weight(40.0f, GovernmentType::ConstitutionalMonarchism),
		Weight(50.0f, GovernmentType::AbsoluteMonarchism),
		Weight(10.0f, GovernmentType::Democracy),
	};

	m_countryGovernment.SetCountry(this);
	m_countryGovernment.SetGovernmentType(random.Select(govTypeChances));

	if (m_countryGovernment.GetGovernmentType() == GovernmentType::AbsoluteMonarchism) {

		GenerateRoyalFamily(random);

		m_countryGovernment.SetTermLength(random.NextInt(3, 11));
		m_countryGovernment.AppointGovernment(random, TimeDate::randomDate(1800 - random.NextInt(1, 6), 1800, random));

	} else if (m_countryGovernment.GetGovernmentType() == GovernmentType::ConstitutionalMonarchism) {

		GenerateRoyalFamily(random);

		GenerateLegislature(random);

		if (m_countryLegislature->HasPowerToElectGovernment()) {
			// no government for now
		} else {
			m_countryGovernment.SetTermLength(m_countryLegislature->GetChamber(true)->GetTermLength());
			m_countryGovernment.AppointGovernment(random, TimeDate::randomDate(1800 - m_countryLegislature->GetChamber(true)->GetTermLength(), 1800, random));
		}

	} else {

		Politician* pPresident = new Politician(Sex::Male, random);
		pPresident->SetSpeciality(PolicyArea::State);
		pPresident->SetTitle(PoliticalTitle::President);

		m_headOfState = pPresident;

		GenerateLegislature(random);

		if (m_countryLegislature->GetChamber(true)->HasMidterms()) {
			m_countryGovernment.SetTermLength(m_countryLegislature->GetChamber(true)->GetTermLength());
		} else {
			m_countryGovernment.SetTermLength(random.NextInt(3, 11));
		}

		// List of chances for different Government Electoral Systems
		std::vector<Weight<GovernmentElectoralSystem>> govElectoralSystems = {
			Weight(15.0f, GovernmentElectoralSystem::WinnerTakesAll),
			Weight(35.0f, GovernmentElectoralSystem::TwoRoundSystem),
			Weight(30.0f, GovernmentElectoralSystem::StateElectoralCollege),
			Weight(20.0f, GovernmentElectoralSystem::ElectoralCollege),
		};

		// Set the election system
		m_countryGovernment.SetElectionSystem(random.Select(govElectoralSystems));

		// Generate the electoral map
		m_countryGovernment.GenerateElectionMap(random);

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

	// Call Demographics Update
	this->UpdateDemographics(pWorld);

	// Call Economy update
	this->UpdateEconomy(pWorld);

	// Call legislature update
	this->UpdateLegislature(pWorld);

	// Call government update
	this->UpdateGovernment(pWorld);

}

void Country::UpdateEconomy(World* pWorld) {

	// Update state economies
	for (auto state : m_states) {
		state->UpdateEconomy();
	}

}

void Country::UpdateDemographics(World* pWorld) {

	// Update state demographics
	for (auto state : m_states) {
		state->UpdateDemographics();
	}

}

void Country::UpdateLegislature(World* pWorld) {

	if (m_countryLegislature) {
		m_countryLegislature->UpdateLegislature(pWorld);
	}

}

void Country::UpdateGovernment(World* pWorld) {
	
	// Is government elected or appointed?
	if (m_countryLegislature && !m_countryLegislature->GetChamber(true)->GetPowers()->canElectGovernment) {

		// Do we have a next elect/appoint date
		if (pWorld->GetDate().isLaterOrSameThan(m_countryGovernment.GetNextFormDate())) {

			// Get government type
			if (m_countryGovernment.GetGovernmentType() == GovernmentType::Democracy) {
			
				// We democratically elect a president
				PresidentialElectionResults electionResult = m_countryGovernment.ElectGovernment(pWorld->GetDate());
			
				// Assign head of state as well
				m_headOfState = m_countryGovernment.GetHeadOfGovernment();

				// Add this event to history
				pWorld->GetHistory()->AddEvent(pWorld->GetDate(), this, EVENT_TYPE::ELECTED_GOVERNMENT, &electionResult);

			} else {
				
				// Appoint the government
				m_countryGovernment.AppointGovernment(pWorld->GetRandom(), pWorld->GetDate());

				// Add this event to history
				pWorld->GetHistory()->AddEvent(pWorld->GetDate(), this, EVENT_TYPE::APPOINT_GOVERNMENT, this->GetGovernment());

			}

		}

	}

	// Do usual government stuff

}

void Country::UpdateRoyalFamily(World* pWorld) {

	// Make sure we actually have a royal family
	if (m_royals) {

		// Update the family
		m_royals->UpdateRoyalFamily();

	}

}

PopSize Country::GetPopulationSize() {
	PopSize size = 0;
	for (auto state : m_states) {
		size += state->GetPopulationSize();
	}
	return size;
}

int Country::GetCityCount() {
	return (int)this->GetCities().size();
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

int Country::GetRegionCount() {

	int count = 0;

	for (auto state : m_states) {
		count += (int)state->GetRegions().size();
	}

	return count;

}

std::vector<Region*> Country::GetRegions() {

	std::vector<Region*> regions;

	for (auto state : m_states) {
		for (auto reg : state->GetRegions()) {
			regions.push_back(reg);
		}
	}

	return regions;

}

std::vector<State*> Country::GetStates() {
	return m_states;
}

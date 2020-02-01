#pragma once
#include "stdlib.h"
#include "State.h"
#include "Region.h"
#include "Government.h"
#include "Legislature.h"
#include "CountryProfile.h"
#include "PoliticalParty.h"

class World;
class Person;
class RoyalFamily;
class Newspaper;
class CountryEconomy;

enum class CountryType {
	Kingdom,
	Republic,
	Federation,
};

class Country {

public:

	struct Party {
		PoliticalParty* party;
		ElectionLevel level;
		Party() {
			party = 0;
			level = ElectionLevel::National;
		}
	};

public:

	Country(std::string name);

	void GenerateCountry(Random random);

	std::string GetName() { return m_countryName; }

	void UpdateCountry(World* pWorld);

	Government* GetGovernment() { return &m_countryGovernment; }
	Legislature* GetLegislature() { return m_countryLegislature; }

	CountryEconomy* GetEconomy() { return m_countryEconomy; }
	CountryProfile* GetProfile() { return &m_countryProfile; }

	Person* GetHeadOfState() { return m_headOfState; }
	RoyalFamily* GetRoyalFamily() { return m_royals; }

	Newspaper* GetNewspaper() { return m_nationalNewspaper; }

	bool IsFederation() { return (int)m_states.size() > 1; }
	bool IsRepublic() { return m_countryGovernment.GetGovernmentType() == GovernmentType::Democracy; }

	PopSize GetPopulationSize();
	int GetCityCount();

	int GetStateCount() { return (int)m_states.size(); }
	int GetRegionCount();

	// SLOW
	City* GetCityByUnsafeIndex(int index);

	std::vector<City*> GetCities();
	std::vector<Region*> GetRegions();
	std::vector<State*> GetStates();

	std::vector<Party> GetPartyList() { return m_parties; }
	bool HasParty(std::string partyname);
	bool hasPartyWithShort(std::string partyshort);
	PoliticalParty* GetPartyByName(std::string partyname);

private:

	void UpdateEconomy(World* pWorld);
	void UpdateLegislature(World* pWorld);
	void UpdateGovernment(World* pWorld);
	void UpdateRoyalFamily(World* pWorld);
	void UpdateDemographics(World* pWorld);

	void GenerateRoyalFamily(Random random);

	void GenerateGeography(Random random);
	void GenerateGovernment(Random random);
	
	void GenerateLegislature(Random random);
	void GeneratePoliticalParties(Random random);

	void GenerateCountryProfile(Random random);
	void GenerateCountryEconomy(Random random);

private:

	std::string m_countryName;

	Government m_countryGovernment;
	Legislature* m_countryLegislature;

	CountryEconomy* m_countryEconomy;
	CountryProfile m_countryProfile;

	Person* m_headOfState;
	RoyalFamily* m_royals;

	Newspaper* m_nationalNewspaper;

	std::vector<State*> m_states;

	std::vector<Party> m_parties;
	
	bool m_useInitialPartyIndex;
	std::vector<std::string> m_partyShorts;

	TimeDate m_lastMonthTime;

	friend class PoliticalParty;

};

#pragma once
#include <map>
#include <vector>
#include <string>
#include "Random.h"
#include "Ideology.h"

enum class PartyColour {
	DARK_RED, RED, PINK, PURPLE,
	DARK_BLUE, BLUE, CYAN, TURQOISE,
	BLACK, YELLOW, GREEN, DARK_GREEN,
	GRAY, ORANGE
};

class City;
class Country;
class Politician;

class PoliticalParty {

private:

	struct PartyMember {
		int electionLvl;
		Politician* pMember;
		Country* pCountry;
		City* pCity;
	};

public:

	PoliticalParty();

	void CreateParty(int establishYear, Country* pCountry, Random random);
	void CreateParty(int establishYear, POLITICAL_IDEOLOGY ideology, Country* pCountry, Random random);

	std::string GetName() { return m_name; }
	std::string GetShort() { return m_short; }
	PartyColour GetColour() { return m_colour; }

	Ideology* GetIdeology() { return &m_ideology; }

	int GetYear() { return m_establishYear; }

	Politician* GetCandidate(int electionLvl, City* pCity);

	void SetBreakout(PoliticalParty* pParty) { m_breakoutFrom = pParty; }
	bool IsBreakoutParty() { return m_breakoutFrom != 0; }

private:

	Politician* FindCandidate(int electionLvl, City* pCity, Country* pCountry);
	Politician* NewCandidate(int electionLvl, City* pCity, Country* pCountry);

	void ConvertNameToShort();
	std::string FindPartyName(Country* pCountry, Random random);

private:

	std::string m_name;
	std::string m_short;
	PartyColour m_colour;

	Ideology m_ideology;

	int m_establishYear;
	Country* m_targetCountry;

	Random m_random;

	PoliticalParty* m_breakoutFrom;

	std::vector<PartyMember> m_members;

};

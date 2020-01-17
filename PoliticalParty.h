#pragma once
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

class Country;

class PoliticalParty {

public:

	PoliticalParty();

	void CreateParty(int establishYear, Country* pCountry, Random random);

	std::string GetName() { return m_name; }
	std::string GetShort() { return m_short; }
	PartyColour GetColour() { return m_colour; }

	Ideology* GetIdeology() { return &m_ideology; }

	int GetYear() { return m_establishYear; }

private:

	void ConvertNameToShort();

private:

	std::string m_name;
	std::string m_short;
	PartyColour m_colour;

	Ideology m_ideology;

	int m_establishYear;
	Country* m_targetCountry;

};

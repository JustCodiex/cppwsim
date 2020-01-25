#pragma once
#include "Random.h"
#include "TimeDate.h"

#include "LegislativeChamber.h"
#include "LegislativeCommittee.h"

class Country;
class World;

class Legislature {

public:

	Legislature();

	void GenerateLegislature(Country* pCountry, Random random);

	bool IsBicameral() { return m_isBicameral; }

	LegislativeChamber* GetChamber(bool firstChamber) {
		if (firstChamber) {
			return m_chambers[0];
		} else {
			return m_chambers[1];
		}
	}

	void UpdateLegislature(World* pWorld);

	void RecalculateElectionDistricts();

	std::string GetName() { return m_name; }

private:

	void UpdateChamber(LegislativeChamber* pChamber, World* pWorld);

	void ElectChamber(LegislativeChamber* pChamber, World* pWorld);

	void GenerateChamberPowers(Country* pCountry, Random random);

private:

	bool m_isBicameral;
	std::string m_name;
	LegislativeChamber* m_chambers[2];

	Country* m_targetCountry;

};

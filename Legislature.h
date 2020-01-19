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

private:

	void UpdateChamber(LegislativeChamber* pChamber, World* pWorld);

	void ElectChamber(LegislativeChamber* pChamber, World* pWorld);

private:

	bool m_isBicameral;
	LegislativeChamber* m_chambers[2];

	Country* m_targetCountry;

};

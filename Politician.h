#pragma once
#include "Person.h"
#include "PolicyArea.h"

class PoliticalParty;
class LegislativeSeat;

class Politician : public Person {

public:

	Politician();
	Politician(Random random);
	Politician(Sex sex, Random random);
	Politician(unsigned char age, Random random);
	Politician(unsigned char age, Sex sex, Random random);

	void SetParty(PoliticalParty* pParty, float loyalty) {
		m_party = pParty;
		m_loyalty = loyalty;
	}

	PoliticalParty* GetParty() { return m_party; }

	bool IsIndependent() { return m_party == 0; }

	LegislativeSeat* GetSeat() { return m_seat; }

	float GetPartyLoyalty() { return m_loyalty; }

	void SetSeat(LegislativeSeat* pSeat) { m_seat = pSeat; }

	void SetSpeciality(PolicyArea area) { m_speciality = area; }
	PolicyArea GetSpeciality() { return m_speciality; }

private:

	PolicyArea m_speciality;
	LegislativeSeat* m_seat;
	PoliticalParty* m_party;
	float m_loyalty;

};

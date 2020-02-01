#pragma once
#include "stdlib.h"
#include "Politician.h"

class LegislativeSeat {

public:

	LegislativeSeat();

	void SetOwner(Politician* pPolitician, TimeDate date);
	void Unseat();

	Politician* GetPolitician() { return m_politician; }
	bool IsVacant() { return m_politician == 0; }

	TimeDate GetElectDate() { return m_electDate; }

	void SetProportional(bool isTrue) { m_isProportionalSeat = isTrue; }
	bool IsProportionalSeat() { return m_isProportionalSeat; }

private:

	bool m_isProportionalSeat;
	Politician* m_politician;
	TimeDate m_electDate;
	
};

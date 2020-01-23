#pragma once
#include "TimeDate.h"
#include "Politician.h"

class LegislativeSeat {

public:

	LegislativeSeat();

	void SetOwner(Politician* pPolitician, TimeDate date);
	void Unseat();

	Politician* GetPolitician() { return m_politician; }
	bool IsVacant() { return m_politician == 0; }

	TimeDate GetElectDate() { return m_electDate; }

private:

	Politician* m_politician;
	TimeDate m_electDate;
	
};

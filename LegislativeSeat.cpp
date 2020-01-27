#include "LegislativeSeat.h"

LegislativeSeat::LegislativeSeat() {
	
	m_politician = 0;
	m_isProportionalSeat = false;

}

void LegislativeSeat::SetOwner(Politician* pPolitician, TimeDate date) {
	m_politician = pPolitician;
	m_politician->SetSeat(this);
	m_politician->SetTitle(PoliticalTitle::MP);
	m_electDate = date;
}

void LegislativeSeat::Unseat() {
	m_politician = 0;
}

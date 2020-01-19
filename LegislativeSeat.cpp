#include "LegislativeSeat.h"

LegislativeSeat::LegislativeSeat() {
	
	m_politician = 0;

}

void LegislativeSeat::SetOwner(Politician* pPolitician, TimeDate date) {
	m_politician = pPolitician;
	m_electDate = date;
}

void LegislativeSeat::Unseat() {
	m_politician = 0;
}

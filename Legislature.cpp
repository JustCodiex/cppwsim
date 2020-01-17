#include "Legislature.h"
#include "Country.h"

Legislature::Legislature() {
	
	// Setup chambers
	m_chambers[0] = NULL;
	m_chambers[1] = NULL;
	m_isBicameral = false;

}

void Legislature::GenerateLegislature(Country* pCountry, Random random) {

	unsigned int popSize = pCountry->GetPopulationSize();

	m_isBicameral = random.NextBool(0.2f);
	int chamberCount = (m_isBicameral) ? 2 : 1;

	for (int i = 0; i < chamberCount; i++) {

		m_chambers[i] = new Chamber;

		unsigned short maxSeats = 0;
		unsigned short minSeats = 0;

		if (popSize > 250000000 && i == 0) {
			maxSeats = 750;
			minSeats = 430;
			m_chambers[i]->isElectable = true;
		} else if (popSize > 10500000 && i == 0) {
			maxSeats = 480;
			minSeats = 350;
			m_chambers[i]->isElectable = true;
		} else if (i == 0) {
			maxSeats = 350;
			minSeats = 68;
			m_chambers[i]->isElectable = true;
		} else {
			if (!pCountry->IsFederation()) {
				m_chambers[i]->eachStateGetsSeats = false;
				m_chambers[i]->isElectable = false;
				maxSeats = 500;
				minSeats = 100;
			} else {
				if (pCountry->IsRepublic()) {
					m_chambers[i]->isElectable = true;
					m_chambers[i]->eachStateGetsSeats = true;
					m_chambers[i]->eachStateSeatCount = random.NextInt(1, 6);
				} else {
					m_chambers[i]->isElectable = false;
					m_chambers[i]->eachStateGetsSeats = false;
					maxSeats = 800;
					minSeats = 200;
				}
			}
		}

		if (i == 0) {
			m_chambers[i]->seatCount = random.NextInt(minSeats, maxSeats + 1);
		} else {
			if (m_chambers[i]->eachStateGetsSeats) {
				m_chambers[i]->seatCount = m_chambers[i]->eachStateSeatCount * (unsigned short)pCountry->GetStateCount();
			} else {
				m_chambers[i]->seatCount = random.NextInt(minSeats, maxSeats + 1);
			}
		}

	}

}

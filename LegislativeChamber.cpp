#include "LegislativeChamber.h"

LegislativeChamber::LegislativeChamber(){
	
	m_chamberName = "Parliament Chamber";
	m_eachStateGetsSeats = false;
	m_eachStateSeatCount = 0;
	m_hasMidTerms = false;
	m_isElectable = false;
	m_seatCount = 0;
	m_termLimit = 0;
	
	// Set all seats to NULL
	memset(m_seats, 0, sizeof(m_seats));

}

void LegislativeChamber::SetSeatCount(unsigned short seats) {
	
	if (seats > 1024) {
		seats = 1024;
	}

	for (int i = 0; i < m_seatCount; i++) {
		if (m_seats[i]) {
			delete m_seats[i];
			m_seats[i] = 0;
		} else {
			break;
		}
	}
	
	m_seatCount = seats;
	
	for (int i = 0; i < m_seatCount; i++) {
		m_seats[i] = new LegislativeSeat;
	}

}

void LegislativeChamber::SetStateData(bool eachState, char seats) {
	m_eachStateGetsSeats = eachState;
	m_eachStateSeatCount = seats;
}

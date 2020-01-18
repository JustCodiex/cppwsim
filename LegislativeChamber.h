#pragma once
#include "TimeDate.h"
#include "LegislativeSeat.h"

class LegislativeChamber {

public:

	LegislativeChamber();

	void SetName(std::string name) { m_chamberName = name; }
	void SetElectable(bool electable) { m_isElectable = electable; }
	void SetTermLimit(int term) { m_termLimit = term; }
	void SetMidtermsEnabled(bool enabled) { m_hasMidTerms = enabled; }

	void SetSeatCount(unsigned short seats);
	void SetStateData(bool eachState, char seats);

	bool IsElectableChamber() { return m_isElectable; }
	bool HasMidterms() { return m_hasMidTerms; }

	bool EachStateGetsRepresentatives() { return m_eachStateGetsSeats; }
	char EachStateRepresentativeCount() { return m_eachStateSeatCount; }

	void CalculateNextElectionDateAndSetLast(TimeDate last) {
		m_lastElection = last;
		m_nextElection = last.addYears(m_termLimit);
	}

	TimeDate GetLastElectionDate() { return m_lastElection; }
	TimeDate GetNextElectionDate() { return m_nextElection; }

	char GetTermLimit() { return m_termLimit; }
	unsigned short GetSeatCount() { return m_seatCount; }

	std::string GetName() { return m_chamberName; }

private:

	char m_termLimit;
	bool m_hasMidTerms;

	unsigned short m_seatCount;
	bool m_isElectable;

	bool m_eachStateGetsSeats;
	char m_eachStateSeatCount;

	TimeDate m_lastElection;
	TimeDate m_nextElection;

	std::string m_chamberName;

	LegislativeSeat* m_seats[1024];

};

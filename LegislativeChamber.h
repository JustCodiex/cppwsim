#pragma once
#include <vector>
#include <map>
#include "TimeDate.h"
#include "LegislativeSeat.h"
#include "ElectoralDistrict.h"

enum class ElectoralSystem {
	ES_FIRST_PAST_THE_POST, // Winner-Takes-All
	ES_PROPORTIONAL,
	ES_TWO_ROUND_SYSTEM,
};

class Country;

class LegislativeChamber {

public:

	struct LegislatureElectionResult {
		bool isMidTerms;
		float turnout;
		unsigned int totalVotes;
		std::map<PoliticalParty*, int> gains;
		std::map<PoliticalParty*, int> seats;
		std::map<PoliticalParty*, float> voteShare;
		std::string chamberName;
	};

public:

	LegislativeChamber();

	void SetName(std::string name) { m_chamberName = name; }
	void SetElectable(bool electable) { m_isElectable = electable; }
	void SetTermLimit(int term) { m_termLimit = term; }
	void SetMidtermsEnabled(bool enabled) { m_hasMidTerms = enabled; }

	void SetSeatCount(unsigned short seats);
	void SetStateData(bool eachState, char seats);

	void SetElectoralSystem(ElectoralSystem system) { m_electoralSystem = system; }

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

	ElectoralSystem GetElectoralSystem() { return m_electoralSystem; }

	void CalculateElectoralDistricts(Country* pCountry);

	LegislatureElectionResult HoldElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate);

	LegislativeSeat* GetSeat(int seatIndex) { return m_seats[seatIndex]; }

private:

	void ClearElectoralDistricts();

	void ElectSeat(int seatIndex, ElectoralDistrictResult& voteResults, LegislatureElectionResult& chamberResults, TimeDate electionDate);

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

	ElectoralSystem m_electoralSystem;
	std::vector<ElectoralDistrict*> m_electoralDistricts;

};

#pragma once
#include <vector>
#include <map>
#include "TimeDate.h"
#include "ElectoralMap.h"
#include "LegislativeSeat.h"
#include "LegislativeElection.h"
#include "LegislativeCoalition.h"

class Country;

const int MAX_SEATS_IN_LEGISLATIVE_CHAMBER = 1080;

class LegislativeChamber {

public:

	struct ChamberPowers {
		bool canRemoveGovernment;
		bool canRepealLaws;
		bool canElectGovernment;
		bool canProposeLaws;
		bool canRejectLaws;
		bool canApproveLaws; // Other chamber sends law for approval only (still becomes law) or government sends law for advisory vote
		bool canApproveAppointments;
	};

public:

	LegislativeChamber();

	void SetName(std::string name) { m_chamberName = name; }
	void SetElectable(bool electable) { m_isElectable = electable; }
	void SetTermLimit(int term) { m_termLength = term; }
	void SetMidtermsEnabled(bool enabled) { m_hasMidTerms = enabled; }

	void SetSeatCount(unsigned short seats);
	void SetStateData(bool eachState, char seats);

	void SetElectoralSystem(ElectoralSystem system) { m_electoralSystem = system; }
	void SetProportionalSystem(ProportionalMethod method) { m_proportionalMethod = method; }
	void SetProportionalSeats(unsigned short seatCount);

	bool IsElectableChamber() { return m_isElectable; }
	bool HasMidterms() { return m_hasMidTerms; }

	bool EachStateGetsRepresentatives() { return m_eachStateGetsSeats; }
	char EachStateRepresentativeCount() { return m_eachStateSeatCount; }

	void CalculateNextElectionDateAndSetLast(TimeDate last) {
		m_lastElection = last;
		m_nextElection = last.addYears(m_termLength);
	}

	TimeDate GetLastElectionDate() { return m_lastElection; }
	TimeDate GetNextElectionDate() { return m_nextElection; }

	char GetTermLength() { return m_termLength; }

	unsigned short GetSeatCount() { return m_seatCount; }
	unsigned short GetSeatForMajority() { return m_seatMajority; }
	unsigned short GetPartySeats(PoliticalParty* party);

	std::string GetName() { return m_chamberName; }

	void SetPowers(ChamberPowers powers) { m_chamberPowers = powers; }
	ChamberPowers* GetPowers() { return &m_chamberPowers; }

	ElectoralSystem GetElectoralSystem() { return m_electoralSystem; }
	ProportionalMethod GetElectoralProportionalMethod() { return m_proportionalMethod; }

	LegislatureElectionResult HoldElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate);

	LegislativeSeat* GetSeat(int seatIndex) { return m_seats[seatIndex]; }

	std::vector<LegislativeCoalition> GetCoalitions() { return m_legislativeCoalitions; }

	bool CanCommandMajority(PoliticalParty* pParty, int& majority);

	PoliticalParty* GetLargestParty();

	PoliticalParty* GetBiggestCoalitionLeader();

	void CalculateElectoralDistricts(Country* pCountry);

	void ElectSeat(int seatIndex, ElectoralDistrictResult& voteResults, LegislatureElectionResult& chamberResults, TimeDate electionDate);
	void ElectSeat(int seatIndex, Politician* pPolitician, LegislatureElectionResult& chamberResults, TimeDate electionDate);

	ElectoralMap* GetElectoralMap() { return m_electoralMap; }

private:

	void FindCoalitions();

private:

	char m_termLength;
	bool m_hasMidTerms;

	unsigned short m_seatMajority;
	unsigned short m_seatCount;
	bool m_isElectable;

	bool m_eachStateGetsSeats;
	char m_eachStateSeatCount;

	TimeDate m_lastElection;
	TimeDate m_nextElection;

	std::string m_chamberName;
	ChamberPowers m_chamberPowers;

	LegislativeSeat* m_seats[MAX_SEATS_IN_LEGISLATIVE_CHAMBER];

	ElectoralSystem m_electoralSystem;
	ProportionalMethod m_proportionalMethod;
	int m_proportionalSeats;

	ElectoralMap* m_electoralMap;
	std::vector<LegislativeCoalition> m_legislativeCoalitions;

};

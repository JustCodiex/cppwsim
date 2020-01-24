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
	ES_FPP_P_MIX,
};

enum class ProportionalMethod {
	PM_NONE,
	PM_DHONDT, // https://en.wikipedia.org/wiki/D%27Hondt_method
	PM_IMPERIALY, // https://en.wikipedia.org/wiki/Imperiali_quota
};

class Country;

const int MAX_SEATS_IN_LEGISLATIVE_CHAMBER = 1080;

class LegislativeChamber {

public:

	struct LegislatureElectionResult {
		bool isMidTerms;
		double turnout;
		unsigned int totalVotes;
		std::map<PoliticalParty*, int> gains;
		std::map<PoliticalParty*, int> seats;
		std::map<PoliticalParty*, double> voteShare;
		std::string chamberName;
		int seatUpForGrabs;
		int seatMajority;
		int seatTotal;
		LegislatureElectionResult() {
			isMidTerms = false;
			turnout = 0;
			totalVotes = 0;
			seatMajority = seatTotal = seatUpForGrabs = 0;
		}
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
	void SetProportionalSystem(ProportionalMethod method) { m_proportionalMethod = method; }
	void SetProportionalSeats(unsigned short seatCount);

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

	LegislatureElectionResult HoldFirstPastThePostElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate);
	LegislatureElectionResult HoldProportionalElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate);
	LegislatureElectionResult HoldTwoRoundSystemElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate);
	LegislatureElectionResult HoldMixedElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate);

	LegislatureElectionResult NewEmptyResults(std::vector<int> seats, Country* pCountry);
	void FinalizeResults(LegislatureElectionResult& results, double divBy);

	std::map<Politician*, int> Proportional_Base(std::vector< ElectoralDistrictResult> districtVotes, LegislatureElectionResult& chamberResults, double& divBy, bool modChamberResults);
	void Proportional_DHondtMethod(unsigned int totalVotes, std::vector<int> seats, std::map<Politician*, int> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate);
	void Proportional_ImperialMethod(unsigned int totalVotes, std::vector<int> seats, std::map<Politician*, int> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate);

	std::vector<ElectoralDistrictResult> CollectVotesFromDistricts(std::vector<int> seats, Country* pCountry);

	void ElectSeat(int seatIndex, ElectoralDistrictResult& voteResults, LegislatureElectionResult& chamberResults, TimeDate electionDate);
	void ElectSeat(int seatIndex, Politician* pPolitician, LegislatureElectionResult& chamberResults, TimeDate electionDate);

private:

	char m_termLimit;
	bool m_hasMidTerms;

	unsigned short m_seatMajority;
	unsigned short m_seatCount;
	bool m_isElectable;

	bool m_eachStateGetsSeats;
	char m_eachStateSeatCount;

	TimeDate m_lastElection;
	TimeDate m_nextElection;

	std::string m_chamberName;

	LegislativeSeat* m_seats[MAX_SEATS_IN_LEGISLATIVE_CHAMBER];

	ElectoralSystem m_electoralSystem;
	ProportionalMethod m_proportionalMethod;
	int m_proportionalSeats;

	std::vector<ElectoralDistrict*> m_electoralDistricts;

};

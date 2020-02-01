#pragma once
#include <map>
#include <vector>
#include <string>
#include "TimeDate.h"

class Ballot;
class Country;
class Politician;
class ElectoralMap;
class PoliticalParty;
class LegislativeChamber;

struct ElectoralDistrictResult;

typedef unsigned long long PopSize;

enum class ElectoralSystem {
	ES_FIRST_PAST_THE_POST, // Winner-Takes-All
	ES_PROPORTIONAL,
	ES_TWO_ROUND_SYSTEM,
	ES_FPP_P_MIX,
};

const std::string ElectoralSystemNames[] = { "First Past the Post", "Proportional", "Two-Round", "FPP and Proportional" };

enum class ProportionalMethod {
	PM_NONE,
	PM_DHONDT, // https://en.wikipedia.org/wiki/D%27Hondt_method
	PM_IMPERIALY, // https://en.wikipedia.org/wiki/Imperiali_quota
};

const std::string ProportionalMethodNames[] = { "", "D'Hondt", "Imperiali Quota" };

struct LegislatureElectionResult {
	bool isMidTerms;
	double turnout;
	PopSize totalVotes;
	ElectoralSystem electoralMethod;
	ProportionalMethod proportionalMethod;
	std::map<PoliticalParty*, int> gains;
	std::map<PoliticalParty*, int> seats;
	std::map<PoliticalParty*, double> voteShare;
	std::string chamberName;
	int seatUpForGrabs;
	int seatMajority;
	int seatTotal;
	int runoffs;
	LegislatureElectionResult() {
		isMidTerms = false;
		turnout = 0;
		totalVotes = 0;
		seatMajority = seatTotal = seatUpForGrabs = 0;
		runoffs = -1;
		electoralMethod = ElectoralSystem::ES_FIRST_PAST_THE_POST;
		proportionalMethod = ProportionalMethod::PM_NONE;
	}
};

namespace LegislativeElection {

	LegislatureElectionResult HoldFirstPastThePostElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate);
	LegislatureElectionResult HoldProportionalElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate);
	LegislatureElectionResult HoldTwoRoundSystemElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate);
	LegislatureElectionResult HoldMixedElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate);

	LegislatureElectionResult NewEmptyResults(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry);
	void FinalizeResults(LegislativeChamber* pChamber, LegislatureElectionResult& results, double divBy);

	std::map<Politician*, PopSize> Proportional_Base(std::vector< ElectoralDistrictResult> districtVotes, LegislatureElectionResult& chamberResults, double& divBy, bool modChamberResults);
	void Proportional_DHondtMethod(LegislativeChamber* pChamber, PopSize totalVotes, std::vector<int> seats, std::map<Politician*, PopSize> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate);
	void Proportional_ImperialMethod(LegislativeChamber* pChamber, PopSize totalVotes, std::vector<int> seats, std::map<Politician*, PopSize> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate);

	std::vector<ElectoralDistrictResult> CollectVotesFromDistricts(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry);

}

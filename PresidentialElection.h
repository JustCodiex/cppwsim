#pragma once
#include "stdlib.h"

class Ballot;
class Country;
class Politician;
class ElectoralMap;

struct PresidentialElectionResults {
	Politician* pWinner;
	double turnout;
	PopSize totalVotes;
	unsigned short electoralCollegeElectors;
	std::map<Politician*, unsigned short> candidateElectoralVotes;
	std::map<Politician*, PopSize> candidateVotes;
	std::map<Politician*, double> candidateVoteShare;
	PresidentialElectionResults() {
		pWinner = 0;
		turnout = 0.0;
		totalVotes = 0;
		electoralCollegeElectors = -1;
	}
};

namespace PresidentialElection {

	PresidentialElectionResults ElectPresident(Country* pCountry, ElectoralMap* pElectoralMap, int system);

	PresidentialElectionResults ElectPresident_PopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap);
	PresidentialElectionResults ElectPresident_TwoRoundPopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap);
	PresidentialElectionResults ElectPresident_ElectoralCollege(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap);

}

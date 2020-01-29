#pragma once
#include <map>
#include <vector>

class Ballot;
class Country;
class Politician;
class ElectoralMap;

struct PresidentialElectionResults {
	Politician* pWinner;
	double turnout;
	unsigned int totalVotes;
	std::map<Politician*, unsigned int> candidateVotes;
	std::map<Politician*, double> candidateVoteShare;
};

namespace PresidentialElection {

	PresidentialElectionResults ElectPresident(Country* pCountry, ElectoralMap* pElectoralMap, int system);

	PresidentialElectionResults ElectPresident_PopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap);
	PresidentialElectionResults ElectPresident_TwoRoundPopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap);
	PresidentialElectionResults ElectPresident_ElectoralCollege(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap);

}

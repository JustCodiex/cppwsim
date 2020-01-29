#include "PresidentialElection.h"
#include "PoliticalParty.h"
#include "ElectoralMap.h"
#include "Country.h"
#include "Ballot.h"

namespace PresidentialElection {

    PresidentialElectionResults ElectPresident(Country* pCountry, ElectoralMap* pElectoralMap, int system) {

        Ballot* presidentialBallot = new Ballot;

        for (auto party : pCountry->GetPartyList()) {
            presidentialBallot->AddCandidate(party.party->GetPrimeCandidate());
        }

        if (system == 2 || system == 3) {

            return ElectPresident_ElectoralCollege(presidentialBallot, pCountry, pElectoralMap);

        } else if (system == 1) {

            return ElectPresident_TwoRoundPopularVote(presidentialBallot, pCountry, pElectoralMap);

        } else if (system == 0) {

            return ElectPresident_PopularVote(presidentialBallot, pCountry, pElectoralMap);

        }

        delete presidentialBallot;

        return PresidentialElectionResults();

    }

    PresidentialElectionResults VoteOnPresidentFromBallot(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap) {

        PresidentialElectionResults results;
        double divBy = 0.0;

        for (auto c : pBallot->GetCandidates()) {
            results.candidateVotes[c] = 0;
        }

        for (size_t i = 0; i < pElectoralMap->GetElectoralDistrictCount(); i++) {

            ElectoralDistrict* pDistrict = pElectoralMap->GetElectoralDistrictByIndex((unsigned int)i);

            if (pDistrict) {

                ElectoralDistrictResult result = pDistrict->CastVotes(pBallot, pCountry);

                results.totalVotes += result.totalVotes;
                results.turnout += result.turnout;

                for (auto v : result.votes) {
                    results.candidateVotes[v.first] += v.second;
                }

            }

            divBy++;

        }

        results.turnout /= divBy;

        for (auto c : pBallot->GetCandidates()) {
            results.candidateVoteShare[c] = results.candidateVotes[c] / (double)results.totalVotes;
        }

        return results;

    }

    PresidentialElectionResults ElectPresident_PopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap) {

        PresidentialElectionResults voteResults = VoteOnPresidentFromBallot(pBallot, pCountry, pElectoralMap);

        return voteResults;

    }

    PresidentialElectionResults ElectPresident_TwoRoundPopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap) {

        PresidentialElectionResults voteResults = VoteOnPresidentFromBallot(pBallot, pCountry, pElectoralMap);

        return voteResults;

    }

    PresidentialElectionResults ElectPresident_ElectoralCollege(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap) {

        return PresidentialElectionResults();

    }

}

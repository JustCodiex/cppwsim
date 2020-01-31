#include "PresidentialElection.h"
#include "PoliticalParty.h"
#include "ElectoralMap.h"
#include "Country.h"
#include "Ballot.h"

namespace PresidentialElection {

    PresidentialElectionResults ElectPresident(Country* pCountry, ElectoralMap* pElectoralMap, int system) {

        PresidentialElectionResults result;
        Ballot* presidentialBallot = new Ballot;

        for (auto party : pCountry->GetPartyList()) {
            presidentialBallot->AddCandidate(party.party->GetPrimeCandidate());
        }

        if (system == 2 || system == 3) {

            result = ElectPresident_ElectoralCollege(presidentialBallot, pCountry, pElectoralMap);

        } else if (system == 1) {

            result = ElectPresident_TwoRoundPopularVote(presidentialBallot, pCountry, pElectoralMap);

        } else {

            result = ElectPresident_PopularVote(presidentialBallot, pCountry, pElectoralMap);

        }

        delete presidentialBallot;

        return result;

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

        // Get votes
        PresidentialElectionResults voteResults = VoteOnPresidentFromBallot(pBallot, pCountry, pElectoralMap);

        // For each candidate
        for (auto vote : voteResults.candidateVotes) {
            
            // If no candidate, or we're better than best candidate
            if (voteResults.pWinner == NULL || vote.second > voteResults.candidateVotes[voteResults.pWinner]) {
                voteResults.pWinner = vote.first;
            }

        }

        // Return finalized results
        return voteResults;

    }

    PresidentialElectionResults ElectPresident_TwoRoundPopularVote(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap) {

        // Get vote results
        PresidentialElectionResults voteResults = VoteOnPresidentFromBallot(pBallot, pCountry, pElectoralMap);

        // Votes to beat
        PopSize bestVote = voteResults.totalVotes / 2;

        Politician* bestCandidate = NULL;
        Politician* secondBestCandidate = NULL;

        // For each candidate
        for (auto vote : voteResults.candidateVotes) {

            // If no candidate, or we're better than best candidate
            if (bestCandidate == NULL || vote.second > voteResults.candidateVotes[bestCandidate]) {
                bestCandidate = vote.first;
                if (vote.second >= bestVote) {
                    voteResults.pWinner = bestCandidate;
                    return voteResults;
                }
            }

            // If no candidate, or we're better than best candidate
            if (secondBestCandidate == NULL || vote.second > voteResults.candidateVotes[secondBestCandidate] && vote.second < voteResults.candidateVotes[bestCandidate]) {
                secondBestCandidate = vote.first;
            }

        }

        // New candidate with two best candidates
        Ballot* runoffBallot = new Ballot;
        runoffBallot->AddCandidate(bestCandidate);
        runoffBallot->AddCandidate(secondBestCandidate);

        // Return the results of a simple popular vote (with the new ballot)
        PresidentialElectionResults r = ElectPresident_PopularVote(runoffBallot, pCountry, pElectoralMap);

        // Delete the runofballot (no longer needed)
        delete runoffBallot;

        // Return results
        return r;

    }

    struct elector {
        float faithfulElector;
        // add more factors
        elector(float faithful) {
            this->faithfulElector = faithful;
        }
    };

    PresidentialElectionResults ElectPresident_ElectoralCollege(Ballot* pBallot, Country* pCountry, ElectoralMap* pElectoralMap) {

        // Random
        Random rand;

        // Electoral Election Results
        PresidentialElectionResults results;
        results.turnout = 0.0f;
        results.totalVotes = 0;
        results.pWinner = 0;
        results.electoralCollegeElectors = (unsigned short)pElectoralMap->GetElectoralDistrictCount();

        // Keep track of electoral pointers
        std::map<Politician*, unsigned short> electoralPointers;

        // Set votes for each candidate
        for (auto c : pBallot->GetCandidates()) {
            results.candidateVotes[c] = 0;
            electoralPointers[c] = 0;
        }

        // Keep track of divisor
        double divBy = 0.0;

        // Generate electors
        for (unsigned int i = 0; i < pElectoralMap->GetElectoralDistrictCount(); i++) {

            // Keep track of the elector
            elector e = elector(1.5f - (pCountry->GetProfile()->countryPoliticalApathy * rand.NextPercentage()));

            // Get votes
            ElectoralDistrictResult districtResult = pElectoralMap->GetElectoralDistrictByIndex(i)->CastVotes(pBallot, pCountry);

            // The winner of the electoral college seat
            Politician* districtWinner = NULL;

            // Add votes to each candidate
            for (auto vote : districtResult.votes) {
                
                // Add votes
                results.candidateVotes[vote.first] += vote.second;
                
                // Did candidate get more votes than current district winner?
                if (districtWinner == NULL || districtResult.votes[vote.first] > districtResult.votes[districtWinner]) {
                    districtWinner = vote.first;
                }

            }

            // Are we a faithful elector
            if (rand.NextBool(e.faithfulElector)) {
                
                // Increment elector support
                electoralPointers[districtWinner]++;

            } else {

                // Get a random
                int rIndex = rand.NextInt(0, pBallot->GetBallotSize());

                // Give them their vote
                electoralPointers[*(pBallot->GetCandidates().begin() + rIndex)]++;

            }

            // Increment total votes
            results.totalVotes += districtResult.totalVotes;

            // Add turnout
            results.turnout += districtResult.turnout;

            // Incement divisor
            divBy++;

        }

        // Calculate vote share
        for (auto c : results.candidateVotes) {
            
            // Update vote share and electoral college votes
            results.candidateVoteShare[c.first] = (double)(c.second / (double)results.totalVotes);
            results.candidateElectoralVotes[c.first] = electoralPointers[c.first];

            // Update electoral college winner
            if (results.pWinner == NULL || electoralPointers[c.first] > electoralPointers[results.pWinner]) {
                results.pWinner = c.first;
            }

        }

        // Get turnout
        results.turnout /= divBy;

        // Return results
        return results;

    }

}

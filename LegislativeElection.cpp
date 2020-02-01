#include "LegislativeElection.h"
#include "LegislativeChamber.h"
#include "PoliticalParty.h"
#include "ElectoralMap.h"
#include "Country.h"
#include "Ballot.h"

namespace LegislativeElection {

	LegislatureElectionResult NewEmptyResults(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry) {

		LegislatureElectionResult result;
		result.chamberName = pChamber->GetName();
		result.isMidTerms = pChamber->HasMidterms();
		result.totalVotes = 0;
		result.turnout = 0.0f;
		result.seatTotal = pChamber->GetSeatCount();
		result.seatUpForGrabs = (int)seats.size();
		result.seatMajority = pChamber->GetSeatForMajority();
		result.runoffs = -1;
		result.electoralMethod = pChamber->GetElectoralSystem();
		result.proportionalMethod = pChamber->GetElectoralProportionalMethod();

		for (auto party : pCountry->GetPartyList()) {
			result.gains[party.party] = 0;
			result.seats[party.party] = 0;
			result.voteShare[party.party] = 0.0f;
		}

		return result;

	}

	void FinalizeResults(LegislativeChamber* pChamber, LegislatureElectionResult& result, double divBy) {

		// Calculate the average turnout
		result.turnout = result.turnout / divBy;

		// Max turnout
		if (result.turnout > 1.0f)
			result.turnout = 1.0f;

		// For all parties in result, calculate average share of vote
		for (auto partyResult : result.voteShare) {
			result.voteShare[partyResult.first] = result.voteShare[partyResult.first] / (double)result.totalVotes;
		}

		// For all seats in chamber
		for (size_t i = 0; i < pChamber->GetSeatCount(); i++) {
			LegislativeSeat* seat = pChamber->GetSeat((int)i);
			if (seat && !seat->IsVacant()) { // if not vacant, increment party seat count
				result.seats[seat->GetPolitician()->GetParty()]++;
			}
		}

	}

	LegislatureElectionResult HoldFirstPastThePostElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

		// Get a new empty result
		LegislatureElectionResult result = NewEmptyResults(pChamber, seats, pCountry);

		// Keep track of the divBy
		double divBy = 0.0;

		// Collect votes from the districts
		std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(pChamber, seats, pCountry);

		// Loop through results
		for (size_t i = 0; i < districtVotes.size(); i++) {

			// Cast votes and get result
			ElectoralDistrictResult results = districtVotes[i];

			// Update chamber results
			result.turnout += results.turnout;
			result.totalVotes += results.totalVotes;

			// Add votes to total vote count
			for (auto share : results.votes) {
				result.voteShare[share.first->GetParty()] += (double)share.second;
			}

			// Elect the seat
			pChamber->ElectSeat(seats[i], results, result, electionDate);

			// Increment divby
			divBy++;

		}

		// Finalize the results
		FinalizeResults(pChamber, result, divBy);

		// Return result
		return result;

	}

	LegislatureElectionResult HoldProportionalElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

		// Get a new empty result
		LegislatureElectionResult result = NewEmptyResults(pChamber, seats, pCountry);

		// Divide by counter
		double divBy = 0;

		// Collect votes from the districts
		std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(pChamber, seats, pCountry);

		// Update vote share
		for (auto distrct : districtVotes) {
			for (auto partyVotes : distrct.votes) {
				result.voteShare[partyVotes.first->GetParty()] += (double)partyVotes.second;
			}
			divBy++;
		}

		// Keep track of votes for each party
		std::map<Politician*, PopSize> voteShare = Proportional_Base(districtVotes, result, divBy, true);

		if (pChamber->GetElectoralProportionalMethod() == ProportionalMethod::PM_DHONDT) {
			Proportional_DHondtMethod(pChamber, result.totalVotes, seats, voteShare, result, electionDate);
		} else if (pChamber->GetElectoralProportionalMethod() == ProportionalMethod::PM_IMPERIALY) {
			Proportional_ImperialMethod(pChamber, result.totalVotes, seats, voteShare, result, electionDate);
		}

		// Finalize the results
		FinalizeResults(pChamber, result, divBy);

		// Return result
		return result;

	}

	LegislatureElectionResult HoldTwoRoundSystemElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

		// Get a new empty result
		LegislatureElectionResult result = NewEmptyResults(pChamber, seats, pCountry);
		result.runoffs = 0;

		// Divide by counter
		double divBy = 0;

		// Collect votes from the districts
		std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(pChamber, seats, pCountry);

		// Remember seat index
		int seatIndex = 0;

		// For all district results
		for (auto districtResult : districtVotes) {

			// Calculate min votes to get elected without a runoff
			PopSize noRunoffVotes = districtResult.totalVotes / 2;

			// No competition, just assign
			if (districtResult.votes.size() == 1) {

				// Increment the total vote count
				result.totalVotes += districtResult.totalVotes;
				result.turnout += districtResult.turnout;

				// Update vote share
				for (auto partyVotes : districtResult.votes) {
					result.voteShare[partyVotes.first->GetParty()] += (double)partyVotes.second;
				}

				// Elect the seat
				pChamber->ElectSeat(seats[seatIndex], (*districtResult.votes.begin()).first, result, electionDate);

			} else {

				Politician* best = NULL; // Candidate with most votes
				Politician* secondBest = NULL; // Candidate with second most votes

				// For all candidates in district
				for (auto candidate : districtResult.votes) {

					// If candidate got more than half of the votes => no runoff, just assign
					if (candidate.second >= noRunoffVotes) {

						// Assign best to second best to candidate and break
						best = secondBest = candidate.first;
						break;

					} else {

						// If best is NULL or this candidate scores better than current best
						if (best == NULL || candidate.second > districtResult.votes[best]) {
							best = candidate.first;
						}

						// If secondbest is NULL or this candidate scores worse than best but better than current second best
						if (secondBest == NULL || (candidate.second < districtResult.votes[best] && candidate.second > districtResult.votes[secondBest])) {
							secondBest = candidate.first;
						}

					}

				}

				// Is best and second best the same => Elect the candidate, no runoff
				if (best == secondBest) {

					// Increment the total vote count
					result.totalVotes += districtResult.totalVotes;
					result.turnout += districtResult.turnout;

					// Update vore share for each candidate => no runoff, count everyone
					for (auto candidate : districtResult.votes) {

						// Update vote count
						result.voteShare[candidate.first->GetParty()] += (double)candidate.second;

					}

					// Elect seat
					pChamber->ElectSeat(seats[seatIndex], best, result, electionDate);

				} else { // Else runoff between the two

					// Increment runoff counter
					result.runoffs++;

					// New ballot with only the two best candidates
					Ballot* runoffBallot = new Ballot;
					runoffBallot->AddCandidate(best);
					runoffBallot->AddCandidate(secondBest);

					// Get runoff election results using new ballot
					ElectoralDistrictResult runoffResult = pChamber->GetElectoralMap()->GetElectoralDistrictByIndex(seatIndex)->CastVotes(runoffBallot, pCountry);

					// Increment the total vote count
					result.totalVotes += runoffResult.totalVotes;
					result.turnout += runoffResult.turnout;

					// Update vote share
					result.voteShare[best->GetParty()] += (double)runoffResult.votes[best];
					result.voteShare[secondBest->GetParty()] += (double)runoffResult.votes[secondBest];

					// If best > secondbest, elect best, otherwise elect secondbest
					if (runoffResult.votes[best] > runoffResult.votes[secondBest]) {
						pChamber->ElectSeat(seats[seatIndex], best, result, electionDate);
					} else {
						pChamber->ElectSeat(seats[seatIndex], secondBest, result, electionDate);
					}

					// Delete the runoff Ballot, no longer needed
					delete runoffBallot;

				}

			}

			// Increment seat/district index
			seatIndex++;

			// Increment div by
			divBy++;

		}

		// Finalize the results
		FinalizeResults(pChamber, result, divBy);

		// Return result
		return result;

	}

	LegislatureElectionResult HoldMixedElection(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

		// Get a new empty result
		LegislatureElectionResult result = NewEmptyResults(pChamber, seats, pCountry);

		// Get proportionl and FPP seats
		std::vector<int> propSeats;
		std::vector<int> fppSeats;

		// For all seats up for grabs
		for (auto seat : seats) {

			// If proportional seat -> add to prop seat list
			if (pChamber->GetSeat(seat)->IsProportionalSeat()) {
				propSeats.push_back(seat);
			} else { // else add to FPP list
				fppSeats.push_back(seat);
			}

		}

		// Divide by counter
		double divBy = 0;

		// Collect votes from the districts
		std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(pChamber, seats, pCountry);

		// Update vote share
		for (auto distrct : districtVotes) {
			for (auto partyVotes : distrct.votes) {
				result.voteShare[partyVotes.first->GetParty()] += (double)partyVotes.second;
			}
			divBy++;
		}

		if (fppSeats.size() > 0) { // The first past the post part

			// Keep track of district indices
			int districtIndex = 0;

			// For all district seats
			for (auto fpp : fppSeats) {

				// Elect the seat
				pChamber->ElectSeat(fpp, districtVotes[districtIndex], result, electionDate);

				// Go to next district
				districtIndex++;

			}

		}

		if (propSeats.size() > 0) { // The proportional part

			// Keep track of votes for each party
			std::map<Politician*, PopSize> voteShare = Proportional_Base(districtVotes, result, divBy, true);

			if (pChamber->GetElectoralProportionalMethod() == ProportionalMethod::PM_DHONDT) {
				Proportional_DHondtMethod(pChamber, result.totalVotes, propSeats, voteShare, result, electionDate);
			} else if (pChamber->GetElectoralProportionalMethod() == ProportionalMethod::PM_IMPERIALY) {
				Proportional_ImperialMethod(pChamber, result.totalVotes, propSeats, voteShare, result, electionDate);
			}

		}

		// Finalize the results
		FinalizeResults(pChamber, result, divBy);

		// Return result
		return result;

	}

	std::map<Politician*, PopSize> Proportional_Base(std::vector< ElectoralDistrictResult> districtVotes, LegislatureElectionResult& chamberResults, double& divBy, bool modChamberResults) {

		// Keep track of votes for each party
		std::map<Politician*, PopSize> voteShare;

		// Loop throgh electoral vote results
		for (size_t i = 0; i < districtVotes.size(); i++) {

			// Cast votes and get result
			ElectoralDistrictResult results = districtVotes[i];

			// Should we modify chamber results
			if (modChamberResults) {

				// Update chamber results
				chamberResults.turnout += results.turnout;
				chamberResults.totalVotes += results.totalVotes;

			}

			// For all candidates
			for (auto candidate : results.votes) {
				voteShare[candidate.first] = candidate.second;
			}

			// Increment divide by
			divBy++;

		}

		return voteShare;

	}

	void Proportional_DHondtMethod(LegislativeChamber* pChamber, PopSize totalVotes, std::vector<int> seats, std::map<Politician*, PopSize> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

		int remainingSeats = (int)seats.size();

		std::map<PoliticalParty*, PopSize> partyVotes;
		std::map<PoliticalParty*, unsigned short> partySeats;
		std::map<PoliticalParty*, std::vector<Politician*>> partyCandidates;

		for (auto candidate : candidateVotes) {

			PoliticalParty* party = candidate.first->GetParty();

			if (partyVotes.find(party) == partyVotes.end()) {
				partyVotes[party] = 0;
				partySeats[party] = 0;
				partyCandidates[party] = std::vector<Politician*>();
			}

			partyVotes[party] += candidate.second;
			partyCandidates[party].push_back(candidate.first);

		}

		while (remainingSeats > 0) {

			std::map<PoliticalParty*, PopSize> partyQuotas;

			for (auto votes : partyVotes) {
				partyQuotas[votes.first] = votes.second / ((PopSize)partySeats[votes.first] + 1);
			}

			PoliticalParty* largestQuotaParty = NULL;

			for (auto quotas : partyQuotas) {
				if (largestQuotaParty == NULL || quotas.second > partyQuotas[largestQuotaParty]) {
					largestQuotaParty = quotas.first;
				}
			}

			partySeats[largestQuotaParty]++;
			partyVotes[largestQuotaParty] /= (PopSize)partySeats[largestQuotaParty] + 1;

			remainingSeats--;

		}

		int seatIndex = 0;

		for (auto seat : partySeats) {

			Politician* bestCandidate = NULL;

			for (auto candidate : partyCandidates[seat.first]) {
				if (bestCandidate == NULL || candidateVotes[candidate] > candidateVotes[bestCandidate]) {
					bestCandidate = candidate;
				}
			}

			pChamber->ElectSeat(seats[seatIndex], bestCandidate, chamberResults, electionDate);

			partyCandidates[seat.first].erase(std::find(partyCandidates[seat.first].begin(), partyCandidates[seat.first].end(), bestCandidate), partyCandidates[seat.first].end());

			seatIndex++;

		}

	}

	void Proportional_ImperialMethod(LegislativeChamber* pChamber, PopSize totalVotes, std::vector<int> seats, std::map<Politician*, PopSize> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

		// The remaining amount of seats to allocate
		PopSize remainingSeats = (PopSize)seats.size();

		// The quota (min votes needed to win)
		PopSize quota = totalVotes / (remainingSeats + 2);

		// Attempts at assigning a seat
		int attemps = 0;

		// List of all elected candidates
		std::vector<Politician*> electedCandidates;

		while (remainingSeats > 0) {

			Politician* largest = NULL;

			for (auto votes : candidateVotes) {
				if (votes.second >= quota) {
					if (largest == NULL || votes.second > candidateVotes[largest]) {
						largest = votes.first;
					}
				}
			}

			if (largest != NULL) {

				electedCandidates.push_back(largest);

				remainingSeats--;

				PopSize leftover = candidateVotes[largest] - quota;

				candidateVotes.erase(candidateVotes.find(largest));

				for (auto nextBest : candidateVotes) { // Single transferable vote
					if (nextBest.first->GetParty() == largest->GetParty()) {
						candidateVotes[nextBest.first] += leftover;
						break;
					}
				}

			} else {

				attemps++;

				if (attemps > remainingSeats + 40 && quota > 0) { // This is a modified version of the Imperiali method => Because we keep reducing the quota
					quota /= 2;
					attemps = 0;
				}

			}

		}

		int seatIndex = 0;

		for (auto pol : electedCandidates) {

			pChamber->ElectSeat(seats[seatIndex], pol, chamberResults, electionDate);

		}

	}

	std::vector<ElectoralDistrictResult> CollectVotesFromDistricts(LegislativeChamber* pChamber, std::vector<int> seats, Country* pCountry) {

		// Store each result from the electoral districts
		std::vector<ElectoralDistrictResult> results;

		// Loop through all the seats up for graps (and make sure the seat is available)
		for (size_t i = 0; i < seats.size() && seats[i] < pChamber->GetElectoralMap()->GetElectoralDistrictCount(); i++) {

			// Get the district
			ElectoralDistrict* pElectoralDistrict = pChamber->GetElectoralMap()->GetElectoralDistrictByIndex((unsigned int)i);

			// Verify it
			if (pElectoralDistrict) {

				// Set the electoral seat
				pElectoralDistrict->SetSeat(pChamber->GetSeat(seats[i]));

				// Create the district ballot
				Ballot* pDistrictBallot = pElectoralDistrict->CreateBallot(pCountry);

				// Cast votes and get result
				ElectoralDistrictResult result = pElectoralDistrict->CastVotes(pDistrictBallot, pCountry);

				// Add result to list of results
				results.push_back(result);

				// Delete the ballot
				delete pDistrictBallot;

			}

		}

		// Return the result
		return results;

	}

}

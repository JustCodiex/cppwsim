#include "LegislativeChamber.h"
#include "Country.h"
#include "Ballot.h"

LegislativeChamber::LegislativeChamber(){
	
	m_chamberName = "Parliament Chamber";
	m_eachStateGetsSeats = false;
	m_eachStateSeatCount = 0;
	m_hasMidTerms = false;
	m_isElectable = false;
	m_seatCount = 0;
	m_termLength = 0;
	
	// Set all seats to NULL
	memset(m_seats, 0, sizeof(m_seats));

}

void LegislativeChamber::SetSeatCount(unsigned short seats) {
	
	if (seats > MAX_SEATS_IN_LEGISLATIVE_CHAMBER) {
		seats = MAX_SEATS_IN_LEGISLATIVE_CHAMBER;
	}

	for (int i = 0; i < m_seatCount; i++) {
		if (m_seats[i]) {
			delete m_seats[i];
			m_seats[i] = 0;
		} else {
			break;
		}
	}
	
	m_seatCount = seats;
	
	for (int i = 0; i < m_seatCount; i++) {
		m_seats[i] = new LegislativeSeat;
	}

	m_seatMajority = m_seatCount / 2;
	if (m_seatMajority % 2 != 0) {
		m_seatMajority++;
	}

}

void LegislativeChamber::SetProportionalSeats(unsigned short seatCount) {

	m_proportionalSeats = seatCount;

	for (unsigned short i = 0; i < seatCount; i++) {
		unsigned short j = m_seatCount - 1 - i;
		m_seats[j]->SetProportional(true);
	}

}

void LegislativeChamber::SetStateData(bool eachState, char seats) {
	m_eachStateGetsSeats = eachState;
	m_eachStateSeatCount = seats;
}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::HoldElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	// The results of the election
	LegislatureElectionResult result;

	if (m_electoralSystem == ElectoralSystem::ES_FIRST_PAST_THE_POST) {

		// Hold a First Past the Post election
		result = HoldFirstPastThePostElection(seats, pCountry, electionDate);

	} else if (m_electoralSystem == ElectoralSystem::ES_PROPORTIONAL) {
		
		// Hold a Proportional Election
		result = HoldProportionalElection(seats, pCountry, electionDate);

	} else if (m_electoralSystem == ElectoralSystem::ES_TWO_ROUND_SYSTEM) {
		
		// Hold a Two Round Election
		result = HoldTwoRoundSystemElection(seats, pCountry, electionDate);

	} else {

		// Hold a Mixed Election
		result = HoldMixedElection(seats, pCountry, electionDate);

	}

	// Lets try and build some coalitions
	FindCoalitions();

	// Return results
	return result;

}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::NewEmptyResults(std::vector<int> seats, Country* pCountry) {

	LegislatureElectionResult result;
	result.chamberName = this->m_chamberName;
	result.isMidTerms = m_hasMidTerms;
	result.totalVotes = 0;
	result.turnout = 0.0f;
	result.seatTotal = m_seatCount;
	result.seatUpForGrabs = (int)seats.size();
	result.seatMajority = m_seatMajority;
	result.runoffs = -1;
	result.electoralMethod = this->m_electoralSystem;
	result.proportionalMethod = this->m_proportionalMethod;

	for (auto party : pCountry->GetPartyList()) {
		result.gains[party.party] = 0;
		result.seats[party.party] = 0;
		result.voteShare[party.party] = 0.0f;
	}

	return result;

}

void LegislativeChamber::FinalizeResults(LegislatureElectionResult& result, double divBy) {

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
	for (size_t i = 0; i < m_seatCount; i++) {
		if (m_seats[i] && !m_seats[i]->IsVacant()) { // if not vacant, increment party seat count
			result.seats[m_seats[i]->GetPolitician()->GetParty()]++;
		}
	}

}

void LegislativeChamber::CalculateElectoralDistricts(Country* pCountry) {

	// District count is the same as seat count (1:1 ratio)
	unsigned short districtCount = 0;

	if (m_electoralSystem == ElectoralSystem::ES_FPP_P_MIX) {
		districtCount = 0;
		for (unsigned short i = 0; i < m_seatCount; i++) {
			if (!m_seats[i]->IsProportionalSeat()) {
				districtCount++;
			}
		}
	} else {
		districtCount = m_seatCount;
	}

	// Does the electoral map already exist?
	if (m_electoralMap != NULL) {
		
		// Clear districts
		m_electoralMap->ClearElectoralDistricts();

		// Delete from memory
		delete m_electoralMap;
		
		// Point to 0 (NULL)
		m_electoralMap = 0;

	}

	m_electoralMap = new ElectoralMap(ElectionLevel::National);
	m_electoralMap->CreateNationalElectoralDistrict(pCountry, districtCount);

}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::HoldFirstPastThePostElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	// Get a new empty result
	LegislatureElectionResult result = NewEmptyResults(seats, pCountry);

	// Keep track of the divBy
	double divBy = 0.0;

	// Collect votes from the districts
	std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(seats, pCountry);

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
		ElectSeat(seats[i], results, result, electionDate);

		// Increment divby
		divBy++;

	}

	// Finalize the results
	FinalizeResults(result, divBy);

	// Return result
	return result;

}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::HoldProportionalElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	// Get a new empty result
	LegislatureElectionResult result = NewEmptyResults(seats, pCountry);

	// Divide by counter
	double divBy = 0;

	// Collect votes from the districts
	std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(seats, pCountry);

	// Update vote share
	for (auto distrct : districtVotes) {
		for (auto partyVotes : distrct.votes) {
			result.voteShare[partyVotes.first->GetParty()] += (double)partyVotes.second;
		}
		divBy++;
	}

	// Keep track of votes for each party
	std::map<Politician*, int> voteShare = Proportional_Base(districtVotes, result, divBy, true);

	if (m_proportionalMethod == ProportionalMethod::PM_DHONDT) {
		Proportional_DHondtMethod(result.totalVotes, seats, voteShare, result, electionDate);
	} else if (m_proportionalMethod == ProportionalMethod::PM_IMPERIALY) {
		Proportional_ImperialMethod(result.totalVotes, seats, voteShare, result, electionDate);
	}

	// Finalize the results
	FinalizeResults(result, divBy);

	// Return result
	return result;

}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::HoldTwoRoundSystemElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	// Get a new empty result
	LegislatureElectionResult result = NewEmptyResults(seats, pCountry);
	result.runoffs = 0;

	// Divide by counter
	double divBy = 0;

	// Collect votes from the districts
	std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(seats, pCountry);

	// Remember seat index
	int seatIndex = 0;

	// For all district results
	for (auto districtResult : districtVotes) {

		// Calculate min votes to get elected without a runoff
		unsigned int noRunoffVotes = districtResult.totalVotes / 2;

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
			ElectSeat(seats[seatIndex], (*districtResult.votes.begin()).first, result, electionDate);

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

				// Update vote count
				result.voteShare[candidate.first->GetParty()] += (double)candidate.second;

			}

			// Is best and second best the same => Elect the candidate, no runoff
			if (best == secondBest) {

				// Increment the total vote count
				result.totalVotes += districtResult.totalVotes;
				result.turnout += districtResult.turnout;

				// Elect seat
				ElectSeat(seats[seatIndex], best, result, electionDate);

			} else { // Else runoff between the two

				// Increment runoff counter
				result.runoffs++;

				// New ballot with only the two best candidates
				Ballot* runoffBallot = new Ballot;
				runoffBallot->AddCandidate(best);
				runoffBallot->AddCandidate(secondBest);

				// Get runoff election results using new ballot
				ElectoralDistrictResult runoffResult = m_electoralMap->GetElectoralDistrictByIndex(seatIndex)->CastVotes(runoffBallot, pCountry);

				// Increment the total vote count
				result.totalVotes += runoffResult.totalVotes;
				result.turnout += runoffResult.turnout;

				// If best > secondbest, elect best, otherwise elect secondbest
				if (runoffResult.votes[best] > runoffResult.votes[secondBest]) {
					ElectSeat(seats[seatIndex], best, result, electionDate);
				} else {
					ElectSeat(seats[seatIndex], secondBest, result, electionDate);
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
	FinalizeResults(result, divBy);

	// Return result
	return result;

}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::HoldMixedElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	// Get a new empty result
	LegislatureElectionResult result = NewEmptyResults(seats, pCountry);

	// Get proportionl and FPP seats
	std::vector<int> propSeats;
	std::vector<int> fppSeats;

	// For all seats up for grabs
	for (auto seat : seats) {

		// If proportional seat -> add to prop seat list
		if (m_seats[seat]->IsProportionalSeat()) {
			propSeats.push_back(seat);
		} else { // else add to FPP list
			fppSeats.push_back(seat);
		}

	}

	// Divide by counter
	double divBy = 0;

	// Collect votes from the districts
	std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(seats, pCountry);

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
			ElectSeat(fpp, districtVotes[districtIndex], result, electionDate);

			// Go to next district
			districtIndex++;

		}

	}

	if (propSeats.size() > 0) { // The proportional part

		// Keep track of votes for each party
		std::map<Politician*, int> voteShare = Proportional_Base(districtVotes, result, divBy, true);

		if (m_proportionalMethod == ProportionalMethod::PM_DHONDT) {
			Proportional_DHondtMethod(result.totalVotes, propSeats, voteShare, result, electionDate);
		} else if (m_proportionalMethod == ProportionalMethod::PM_IMPERIALY) {
			Proportional_ImperialMethod(result.totalVotes, propSeats, voteShare, result, electionDate);
		}

	}

	// Finalize the results
	FinalizeResults(result, divBy);

	// Return result
	return result;

}

std::map<Politician*, int> LegislativeChamber::Proportional_Base(std::vector< ElectoralDistrictResult> districtVotes, LegislatureElectionResult& chamberResults, double& divBy, bool modChamberResults) {

	// Keep track of votes for each party
	std::map<Politician*, int> voteShare;

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

void LegislativeChamber::Proportional_DHondtMethod(unsigned int totalVotes, std::vector<int> seats, std::map<Politician*, int> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

	int remainingSeats = (int)seats.size();

	std::map<PoliticalParty*, unsigned int> partyVotes;
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
	
		std::map<PoliticalParty*, unsigned int> partyQuotas;

		for (auto votes : partyVotes) {
			partyQuotas[votes.first] = votes.second / (partySeats[votes.first] + 1);
		}

		PoliticalParty* largestQuotaParty = NULL;

		for (auto quotas : partyQuotas) {
			if (largestQuotaParty == NULL || quotas.second > partyQuotas[largestQuotaParty]) {
				largestQuotaParty = quotas.first;
			}
		}

		partySeats[largestQuotaParty]++;
		partyVotes[largestQuotaParty] /= partySeats[largestQuotaParty] + 1;

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

		ElectSeat(seats[seatIndex], bestCandidate, chamberResults, electionDate);

		partyCandidates[seat.first].erase(std::find(partyCandidates[seat.first].begin(), partyCandidates[seat.first].end(), bestCandidate), partyCandidates[seat.first].end());

		seatIndex++;

	}

}

void LegislativeChamber::Proportional_ImperialMethod(unsigned int totalVotes, std::vector<int> seats, std::map<Politician*, int> candidateVotes, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

	// The remaining amount of seats to allocate
	int remainingSeats = (int)seats.size();

	// The quota (min votes needed to win)
	int quota = totalVotes / (remainingSeats + 2);

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

			int leftover = candidateVotes[largest] - quota;

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

		ElectSeat(seats[seatIndex], pol, chamberResults, electionDate);

	}

}

std::vector<ElectoralDistrictResult> LegislativeChamber::CollectVotesFromDistricts(std::vector<int> seats, Country* pCountry) {

	// Store each result from the electoral districts
	std::vector<ElectoralDistrictResult> results;

	// Loop through all the seats up for graps (and make sure the seat is available)
	for (size_t i = 0; i < seats.size() && seats[i] < m_electoralMap->GetElectoralDistrictCount(); i++) {

		// Get the district
		ElectoralDistrict* pElectoralDistrict = m_electoralMap->GetElectoralDistrictByIndex((unsigned int)i);

		// Verify it
		if (pElectoralDistrict) {

			// Set the electoral seat
			pElectoralDistrict->SetSeat(m_seats[seats[i]]);

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

void LegislativeChamber::ElectSeat(int seatIndex, ElectoralDistrictResult& voteResults, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

	Politician* pol = NULL;

	for (auto result : voteResults.votes) {
		
		if (pol == NULL || result.second > voteResults.votes[result.first]) {
			pol = result.first;
		}

	}

	// Elect the seat
	ElectSeat(seatIndex, pol, chamberResults, electionDate);

}

void LegislativeChamber::ElectSeat(int seatIndex, Politician* pPolitician, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

	if (pPolitician != NULL) {

		if (!m_seats[seatIndex]->IsVacant() && m_seats[seatIndex]->GetPolitician()->GetParty() != pPolitician->GetParty()) {
			chamberResults.gains[m_seats[seatIndex]->GetPolitician()->GetParty()]--;
			chamberResults.gains[pPolitician->GetParty()]++;
		} else if (m_seats[seatIndex]->IsVacant()) {
			chamberResults.gains[pPolitician->GetParty()]++;
		}

		m_seats[seatIndex]->SetOwner(pPolitician, electionDate);

	}

}

void LegislativeChamber::FindCoalitions() {

	// Clear any pre-existing coalitions
	m_legislativeCoalitions.clear();

	// Store the seat count for all parties
	std::map<PoliticalParty*, int> partySeats;

	// For each seat in chamber
	for (size_t i = 0; i < m_seatCount; i++) {

		// Make sure seat is not vacant and is valid
		if (m_seats[i] && !m_seats[i]->IsVacant()) {

			// Get a reference to the party of the seat owner
			PoliticalParty* party = m_seats[i]->GetPolitician()->GetParty();

			// Can we find the party in our map?
			if (partySeats.find(party) == partySeats.end()) {
				partySeats[party] = 1;
			} else {
				partySeats[party]++;
			}

		}

	}

	// For each party in chamber
	for (auto party : partySeats) {

		// Simmple coalition with party only
		LegislativeCoalition simpleCoalition;
		simpleCoalition.seats = party.second;
		simpleCoalition.leader = party.first;
		simpleCoalition.seatMajority = simpleCoalition.seats - m_seatMajority;
		simpleCoalition.type = (simpleCoalition.seats >= m_seatMajority) ? CoalitionType::Majority : CoalitionType::Opposition;
		simpleCoalition.name = party.first->GetName();
		simpleCoalition.conflictingPartners = 0;
		simpleCoalition.stability = 1.0f;

		// Add coalition to legislative coalitions
		m_legislativeCoalitions.push_back(simpleCoalition);

		// Copy coalition into the additive coalition
		LegislativeCoalition formerCoalition = LegislativeCoalition(&simpleCoalition);
		formerCoalition.name = formerCoalition.leader->GetShort();

		// For all other parties in chambe
		for (auto otherParties : partySeats) {

			// Make sure we're not working with ourselves
			if (otherParties.first != party.first) {

				// Make sure we're compatible with other party
				if (party.first->GetIdeology()->IsCompatible(otherParties.first->GetIdeology())) {

					// Make sure other party is compatible with the rest of the coalition
					bool canWorkWithOtherCoalitionPartners = true;
					for (auto partner : formerCoalition.partners) {
						if (!otherParties.first->GetIdeology()->IsCompatible(partner->GetIdeology())) {
							canWorkWithOtherCoalitionPartners = false;
							break;
						}
					}

					// Add party to coalition
					LegislativeCoalition col = LegislativeCoalition(&formerCoalition);
					col.partners.push_back(otherParties.first);
					col.seats += otherParties.second;
					col.seatsSupplied += otherParties.second;
					col.seatMajority = col.seats - m_seatMajority;
					col.name += otherParties.first->GetShort();
					col.type = (col.seats >= m_seatMajority) ? CoalitionType::Minority : CoalitionType::Opposition;

					// Is this coalition sorta unstable with unstable partners?
					if (!canWorkWithOtherCoalitionPartners) {
						col.conflictingPartners++;
						col.stability = 1.0f - (col.conflictingPartners / (float)col.partners.size());
					}
					// Add coalition option
					m_legislativeCoalitions.push_back(col);

					// Update former coalition to reflect new coalition possibility
					formerCoalition = col;

				}

			}

		}

	}

}

unsigned short LegislativeChamber::GetPartySeats(PoliticalParty* party) {

	unsigned short count = 0;

	// For each seat in chamber
	for (size_t i = 0; i < m_seatCount; i++) {

		// Make sure seat is not vacant and is valid
		if (m_seats[i] && !m_seats[i]->IsVacant()) {

			// Is party we're looking for?
			if (m_seats[i]->GetPolitician()->GetParty() == party) {
				count++;
			}

		}

	}

	return count;

}

bool LegislativeChamber::CanCommandMajority(PoliticalParty* pParty, int& majority) {

	for (LegislativeCoalition coalition : m_legislativeCoalitions) {

		if (coalition.type != CoalitionType::Opposition) {
			if (coalition.leader == pParty || std::find(coalition.partners.begin(), coalition.partners.end(), pParty) != coalition.partners.end()) {
				majority = coalition.seatMajority;
				return true;
			}
		} 

	}

	return false;

}

PoliticalParty* LegislativeChamber::GetLargestParty() {
	
	std::map<PoliticalParty*, unsigned short> partySeats;

	// For each seat in chamber
	for (size_t i = 0; i < m_seatCount; i++) {

		// Make sure seat is not vacant and is valid
		if (m_seats[i] && !m_seats[i]->IsVacant()) {

			// Is party we're looking for?
			if (m_seats[i]->GetPolitician()->GetParty() != NULL) {
				partySeats[m_seats[i]->GetPolitician()->GetParty()]++;
			}

		}

	}

	PoliticalParty* largestParty = NULL;

	for (auto party : partySeats) {
		if (largestParty == NULL || party.second > partySeats[party.first]) {
			largestParty = party.first;
		}
	}

	return largestParty;

}

PoliticalParty* LegislativeChamber::GetBiggestCoalitionLeader() {

	LegislativeCoalition coalition;

	for (auto col : m_legislativeCoalitions) {
		if (col.seats > coalition.seats) {
			coalition = col;
		}
	}

	return coalition.leader;

}

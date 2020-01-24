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
	m_termLimit = 0;
	
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

void LegislativeChamber::ClearElectoralDistricts() {

	// Loop through all the districts
	for (size_t i = 0; i < m_electoralDistricts.size(); i++) {

		// Make sure it's valid
		if (m_electoralDistricts[i]) {
			
			// Delete it from memory
			delete m_electoralDistricts[i];

		}

	}

	// Clear
	m_electoralDistricts.clear();

}

void LegislativeChamber::CalculateElectoralDistricts(Country* pCountry) {

	// Clear district
	this->ClearElectoralDistricts();

	// District count is the same as seat count (1:1 ratio)
	int districtCount;

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

	// Get cities
	std::vector<City*> cities = pCountry->GetCities();

	// For all cities in country
	for (size_t i = 0; i < cities.size(); i++) {

		// Create new district
		ElectoralDistrict* pDistrict = new ElectoralDistrict(ElectionLevel::National);
		pDistrict->SetDistrict(cities[i], 1.0f, pCountry->GetProfile());

		// Add district
		m_electoralDistricts.push_back(pDistrict);

	}

	// Handle cases where there are more or less seats than cities
	if (cities.size() < districtCount) { // We split large cities

		// As long as we have less than the districts we need
		while (m_electoralDistricts.size() < districtCount) {

			// Index of largest city
			int largest = 0;

			// For all city districts
			for (size_t i = 1; i < m_electoralDistricts.size(); i++) {

				unsigned int thisPop = m_electoralDistricts[i]->GetVoterCount();
				unsigned int largestPop = m_electoralDistricts[largest]->GetVoterCount();

				// Is this city larger than found largest?
				if (thisPop > largestPop) {
					largest = (int)i; // Update
				}
			}

			// Splt largest district
			ElectoralDistrict* pNewDistrict = m_electoralDistricts[largest]->Split(pCountry->GetProfile());

			// Add new district
			m_electoralDistricts.push_back(pNewDistrict);

		}

	} else if (cities.size() > districtCount) { // We merge small cities

		// As long as we have more districts than we need
		while (m_electoralDistricts.size() > districtCount) {
			
			// Index of smallest city
			size_t smallest = 0;

			// Index of next-smallest city
			size_t nextSmallest = 1;

			// For all cities
			for (size_t i = 0; i < m_electoralDistricts.size(); i++) {

				// Get voters in district
				unsigned int voters = m_electoralDistricts[i]->GetVoterCount();

				// If v[smallest] < v[i] < v[nextSmallest] then update nextsmallest
				if (voters > m_electoralDistricts[smallest]->GetVoterCount() && voters < m_electoralDistricts[nextSmallest]->GetVoterCount()) {
					nextSmallest = i;
				} else if (voters < m_electoralDistricts[smallest]->GetVoterCount()) { // else if smaller than smallest

					// Update next smallest
					nextSmallest = smallest;

					// Udpate smallest
					smallest = i;

				}

			}

			// Get pointers
			ElectoralDistrict* pSmallestDistrict = m_electoralDistricts[smallest];
			ElectoralDistrict* pNextSmallestDistrict = m_electoralDistricts[nextSmallest];

			// Merge districts
			ElectoralDistrict* pNewDistrict = ElectoralDistrict::MergeAndDelete(pSmallestDistrict, pNextSmallestDistrict, pCountry->GetProfile());

			// Remove smallest from electoral list
			m_electoralDistricts.erase(m_electoralDistricts.begin() + smallest);

			// Remove next smallest (and we might have to subtract index by one if greater than smallest.
			if (nextSmallest > smallest) {
				m_electoralDistricts.erase(m_electoralDistricts.begin() + (nextSmallest - (size_t)1));
			} else {
				m_electoralDistricts.erase(m_electoralDistricts.begin() + nextSmallest);
			}

			if (pSmallestDistrict) {
				delete pSmallestDistrict;
			}

			if (pNextSmallestDistrict) {
				delete pNextSmallestDistrict;
			}

			// Add merged districts
			m_electoralDistricts.push_back(pNewDistrict);

		}

	}

}

LegislativeChamber::LegislatureElectionResult LegislativeChamber::HoldElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	if (m_electoralSystem == ElectoralSystem::ES_FIRST_PAST_THE_POST) {

		// Hold a First Past the Post election
		return HoldFirstPastThePostElection(seats, pCountry, electionDate);

	} else if (m_electoralSystem == ElectoralSystem::ES_PROPORTIONAL) {
		
		// Hold a Proportional Election
		return HoldProportionalElection(seats, pCountry, electionDate);

	} else if (m_electoralSystem == ElectoralSystem::ES_TWO_ROUND_SYSTEM) {
		
		// Hold a Two Round Election
		return HoldTwoRoundSystemElection(seats, pCountry, electionDate);

	} else {

		// Hold a Mixed Election
		return HoldMixedElection(seats, pCountry, electionDate);

	}

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
		result.voteShare[partyResult.first] = result.voteShare[partyResult.first] / divBy;
	}

	// For all seats in chamber
	for (size_t i = 0; i < m_seatCount; i++) {
		if (m_seats[i] && !m_seats[i]->IsVacant()) { // if not vacant, increment party seat count
			result.seats[m_seats[i]->GetPolitician()->GetParty()]++;
		}
	}

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

	// Divide by counter
	double divBy = 0;

	// Collect votes from the districts
	std::vector<ElectoralDistrictResult> districtVotes = CollectVotesFromDistricts(seats, pCountry);

	// Remember seat index
	int seatIndex = 0;

	// For all district results
	for (auto districtResult : districtVotes) {

		// Calculate min votes to get elected without a runoff
		int noRunoffVotes = districtResult.totalVotes / 2;

		// No competition, just assign
		if (districtResult.votes.size() == 1) {
			ElectSeat(seats[seatIndex], (*districtResult.votes.begin()).first, result, electionDate);
		} else {

			Politician* best = NULL;
			Politician* secondBest = NULL;

			for (auto candidate : districtResult.votes) {
				if (candidate.second >= noRunoffVotes) {
					best = secondBest = candidate.first;
					break;
				} else {
					if (best == NULL || candidate.second > districtResult.votes[best]) {
						best = candidate.first;
					}
					if (secondBest == NULL || (candidate.second < districtResult.votes[best] && candidate.second > districtResult.votes[secondBest])) {
						secondBest = candidate.first;
					}
				}
			}

			if (best == secondBest) {

				ElectSeat(seats[seatIndex], best, result, electionDate);

			} else {

				Ballot* runoffBallot = new Ballot;
				runoffBallot->AddCandidate(best);
				runoffBallot->AddCandidate(secondBest);

				ElectoralDistrictResult runoffResult = m_electoralDistricts[seatIndex]->CastVotes(runoffBallot, pCountry);

				if (runoffResult.votes[best] > runoffResult.votes[secondBest]) {
					ElectSeat(seats[seatIndex], best, result, electionDate);
				} else {
					ElectSeat(seats[seatIndex], secondBest, result, electionDate);
				}

				delete runoffBallot;

			}

		}

		seatIndex++;

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
	
		std::map<PoliticalParty*, int> partyQuotas;

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

			if (attemps > remainingSeats + 40 && quota > 0) { // This is a modified version of the Imperial method => Because we keep reducing the quota
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
	for (size_t i = 0; i < seats.size() && seats[i] < m_electoralDistricts.size(); i++) {

		// Get the district
		ElectoralDistrict* pElectoralDistrict = m_electoralDistricts[seats[i]];

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
		
		chamberResults.voteShare[result.first->GetParty()] += voteResults.voteshare[result.first];

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

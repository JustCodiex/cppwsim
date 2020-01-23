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
	
	if (seats > 1024) {
		seats = 1024;
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
	int districtCount = m_seatCount;

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

	LegislatureElectionResult result;
	result.chamberName = this->m_chamberName;
	result.isMidTerms = m_hasMidTerms;
	result.totalVotes = 0;
	result.turnout = 0.0f;

	for (auto party : pCountry->GetPartyList()) {
		result.gains[party.party] = 0;
		result.seats[party.party] = 0;
		result.voteShare[party.party] = 0.0f;
	}

	float divBy = 0;

	// Loop through all the seats up for graps (and make sure the seat is available)
	for (size_t i = 0; i < seats.size() && seats[i] < m_electoralDistricts.size(); i++) {

		// Get the district
		ElectoralDistrict* pElectoralDistrict = m_electoralDistricts[seats[i]];

		// Verify it
		if (pElectoralDistrict) {

			// Create the district ballot
			Ballot* pDistrictBallot = pElectoralDistrict->CreateBallot(pCountry);
			
			// Cast votes and get result
			ElectoralDistrictResult results = pElectoralDistrict->CastVotes(pDistrictBallot, pCountry);

			// Update chamber results
			result.turnout += results.turnout;
			result.totalVotes += results.totalVotes;

			// Elect the seat
			ElectSeat((int)i, results, result, electionDate);

			// Increment divby
			divBy++;

		}

	}

	// Calculate the average turnout
	result.turnout = result.turnout / divBy;

	for (auto partyResult : result.voteShare) {
		result.voteShare[partyResult.first] = result.voteShare[partyResult.first] / divBy;
	}

	for (size_t i = 0; i < m_seatCount; i++) {
		if (m_seats[i]) {
			if (!m_seats[i]->IsVacant()) {
				result.seats[m_seats[i]->GetPolitician()->GetParty()]++;
			}
		}
	}

	return result;

}

void LegislativeChamber::ElectSeat(int seatIndex, ElectoralDistrictResult& voteResults, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

	Politician* pol = NULL;

	for (auto result : voteResults.votes) {
		
		chamberResults.voteShare[result.first->GetParty()] += voteResults.voteshare[result.first];

		if (pol == NULL || result.second > voteResults.votes[result.first]) {
			pol = result.first;
		}

	}

	if (pol != NULL) {
		
		if (!m_seats[seatIndex]->IsVacant() && m_seats[seatIndex]->GetPolitician()->GetParty() != pol->GetParty()) {
			chamberResults.gains[m_seats[seatIndex]->GetPolitician()->GetParty()]--;
			chamberResults.gains[pol->GetParty()]++;
		} else if (m_seats[seatIndex]->IsVacant()) {
			chamberResults.gains[pol->GetParty()]++;
		}

		m_seats[seatIndex]->SetOwner(pol, electionDate);

	}

}

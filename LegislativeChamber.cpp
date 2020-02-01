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

LegislatureElectionResult LegislativeChamber::HoldElection(std::vector<int> seats, Country* pCountry, TimeDate electionDate) {

	// The results of the election
	LegislatureElectionResult result;

	if (m_electoralSystem == ElectoralSystem::ES_FIRST_PAST_THE_POST) {

		// Hold a First Past the Post election
		result = LegislativeElection::HoldFirstPastThePostElection(this, seats, pCountry, electionDate);

	} else if (m_electoralSystem == ElectoralSystem::ES_PROPORTIONAL) {
		
		// Hold a Proportional Election
		result = LegislativeElection::HoldProportionalElection(this, seats, pCountry, electionDate);

	} else if (m_electoralSystem == ElectoralSystem::ES_TWO_ROUND_SYSTEM) {
		
		// Hold a Two Round Election
		result = LegislativeElection::HoldTwoRoundSystemElection(this, seats, pCountry, electionDate);

	} else {

		// Hold a Mixed Election
		result = LegislativeElection::HoldMixedElection(this, seats, pCountry, electionDate);

	}

	// Lets try and build some coalitions
	FindCoalitions();

	// Return results
	return result;

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

void LegislativeChamber::ElectSeat(int seatIndex, ElectoralDistrictResult& voteResults, LegislatureElectionResult& chamberResults, TimeDate electionDate) {

	Politician* pol = NULL;

	for (auto result : voteResults.votes) {
		
		if (pol == NULL || result.second > voteResults.votes[pol]) {
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

	// Biggest coalition
	LegislativeCoalition coalition;

	for (auto col : m_legislativeCoalitions) {
		
		// Does the coalition have more seats than current coalition?
		if (col.seats > coalition.seats) {
			coalition = col; // Update biggest coalition
		}

	}

	// Return the leader of the biggest coalition
	return coalition.leader;

}

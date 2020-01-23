#include "Legislature.h"
#include "Country.h"
#include "Ballot.h"
#include "World.h"
#include "History.h"
#include "Newspaper.h"
#include <iostream>

Legislature::Legislature() {
	
	// Setup chambers
	m_chambers[0] = NULL;
	m_chambers[1] = NULL;
	m_isBicameral = false;

	m_targetCountry = 0;

}

void Legislature::GenerateLegislature(Country* pCountry, Random random) {

	m_targetCountry = pCountry;

	unsigned int popSize = pCountry->GetPopulationSize();

	m_isBicameral = random.NextBool(0.2f);
	int chamberCount = (m_isBicameral) ? 2 : 1;

	std::vector<Weight< ElectoralSystem>> systemChances = {
		Weight(45.0f, ElectoralSystem::ES_FIRST_PAST_THE_POST),
		Weight(35.0f, ElectoralSystem::ES_PROPORTIONAL),
		Weight(20.0f, ElectoralSystem::ES_TWO_ROUND_SYSTEM),
	};

	for (int i = 0; i < chamberCount; i++) {

		m_chambers[i] = new LegislativeChamber;
		m_chambers[i]->SetElectoralSystem(random.Select(systemChances));

		unsigned short maxSeats = 0;
		unsigned short minSeats = 0;

		if (popSize > 250000000 && i == 0) {
			maxSeats = 750;
			minSeats = 430;
			m_chambers[i]->SetElectable(true);
		} else if (popSize > 10500000 && i == 0) {
			maxSeats = 480;
			minSeats = 350;
			m_chambers[i]->SetElectable(true);
		} else if (i == 0) {
			maxSeats = 350;
			minSeats = 68;
			m_chambers[i]->SetElectable(true);
		} else {
			if (!pCountry->IsFederation()) {
				m_chambers[i]->SetElectable(false);
				maxSeats = 500;
				minSeats = 100;
			} else {
				if (pCountry->IsRepublic()) {
					m_chambers[i]->SetElectable(true);
					m_chambers[i]->SetStateData(true, random.NextInt(1, 6));
				} else {
					m_chambers[i]->SetElectable(false);
					maxSeats = 800;
					minSeats = 200;
				}
			}
		}

		if (chamberCount > 1) {
			m_chambers[i]->SetMidtermsEnabled(random.NextBool(0.5f));
			m_chambers[i]->SetTermLimit(random.NextInt(1, 5));
		} else {
			m_chambers[i]->SetTermLimit(random.NextInt(2, 8));
		}

		m_chambers[i]->CalculateNextElectionDateAndSetLast(TimeDate::randomDate(1800 - m_chambers[i]->GetTermLimit(), 1800, random));

		if (i == 0 || !m_chambers[i]->EachStateGetsRepresentatives()) {
			m_chambers[i]->SetSeatCount((unsigned short)random.NextInt(minSeats, maxSeats + 1));
		} else {
			m_chambers[i]->SetSeatCount(m_chambers[i]->EachStateRepresentativeCount() * (unsigned short)pCountry->GetStateCount());
		}

	}

}

void Legislature::RecalculateElectionDistricts() {

	if (m_chambers[0])
		m_chambers[0]->CalculateElectoralDistricts(m_targetCountry);

	if (m_chambers[1])
		m_chambers[1]->CalculateElectoralDistricts(m_targetCountry);

}

void Legislature::UpdateLegislature(World* pWorld) {

	if (m_chambers[0])
		UpdateChamber(m_chambers[0], pWorld);

	if (m_chambers[1])
		UpdateChamber(m_chambers[1], pWorld);

}

void Legislature::UpdateChamber(LegislativeChamber* pChamber, World* pWorld) {

	if (pChamber->IsElectableChamber() && pWorld->GetDate().isDate(pChamber->GetNextElectionDate())) {
		
		// Elect the chamber
		ElectChamber(pChamber, pWorld);

	} else {

		// Process laws/committees

	}

}

void Legislature::ElectChamber(LegislativeChamber* pChamber, World* pWorld) {

	// Update election date
	pChamber->CalculateNextElectionDateAndSetLast(pWorld->GetDate());

	// Seats to elect
	std::vector<int> seats;

	// Determine the type of election to have
	if (pChamber->HasMidterms()) {

		// Calculate the max amount of seats we'll allow to be up for graps.
		// We're doing it like this to make sure we force midterm elections.
		int maxSeats = (pChamber->GetSeatCount() / 2) + 1;

		// Go through all the available seats
		for (int i = 0; i < pChamber->GetSeatCount() && seats.size() < maxSeats; i++) {

			// Get seat
			LegislativeSeat* pSeat = pChamber->GetSeat(i);
			if (pSeat) {

				// Are we supposed to elect this seat?
				if (pWorld->GetDate().isLaterOrSameThan(pSeat->GetElectDate().addYears(pChamber->GetTermLimit()))) {

					// Add seat
					seats.push_back(i);

				}

			}

		}

	} else {

		// For all seats in chamber
		for (int i = 0; i < pChamber->GetSeatCount(); i++) {

			// Add seat to election
			seats.push_back(i);

		}

	}

	// Hold the election
	LegislativeChamber::LegislatureElectionResult electionResult = pChamber->HoldElection(seats, m_targetCountry, pWorld->GetDate());
	pWorld->GetHistory()->AddEvent(pWorld->GetDate(), m_targetCountry, EVENT_TYPE::ELECTION_LEGISLATURE, (void*)&electionResult);

	m_targetCountry->GetNewspaper()->LegislatureElectionResults(pWorld->GetDate(), electionResult);

}

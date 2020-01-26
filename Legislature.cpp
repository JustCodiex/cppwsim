#include "Legislature.h"
#include "Country.h"
#include "Ballot.h"
#include "World.h"
#include "History.h"
#include "Newspaper.h"
#include "name_legislatures.h"
#include "name_chambers.h"
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

	m_name = GetRandomLegislatureName(random);

	std::vector<Weight< ElectoralSystem>> systemChances = {
		Weight(55.0f, ElectoralSystem::ES_FIRST_PAST_THE_POST),
		Weight(5.0f, ElectoralSystem::ES_PROPORTIONAL),
		Weight(25.0f, ElectoralSystem::ES_TWO_ROUND_SYSTEM),
		Weight(20.0f, ElectoralSystem::ES_FPP_P_MIX),
	};

	for (int i = 0; i < chamberCount; i++) {

		m_chambers[i] = new LegislativeChamber;
		m_chambers[i]->SetName((m_isBicameral) ? GetChamberName(random, i == 0) : this->m_name);

		ElectoralSystem electSys = random.Select(systemChances);
		m_chambers[i]->SetElectoralSystem(electSys);

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
			m_chambers[i]->SetMidtermsEnabled((electSys != ElectoralSystem::ES_FPP_P_MIX && electSys != ElectoralSystem::ES_PROPORTIONAL) ? random.NextBool(0.5f) : false);
			m_chambers[i]->SetTermLimit(random.NextInt(1, 5));
		} else {
			m_chambers[i]->SetTermLimit(random.NextInt(2, 8));
		}

		m_chambers[i]->CalculateNextElectionDateAndSetLast(TimeDate::randomDate(1800 - m_chambers[i]->GetTermLength(), 1800, random));

		if (i == 0 || !m_chambers[i]->EachStateGetsRepresentatives()) {
			m_chambers[i]->SetSeatCount((unsigned short)random.NextInt(minSeats, maxSeats + 1));
		} else {
			m_chambers[i]->SetSeatCount(m_chambers[i]->EachStateRepresentativeCount() * (unsigned short)pCountry->GetStateCount());
		}

		if (electSys == ElectoralSystem::ES_PROPORTIONAL) {
			ProportionalMethod pMeth[] = { ProportionalMethod::PM_DHONDT, ProportionalMethod::PM_IMPERIALY };
			m_chambers[i]->SetProportionalSystem(pMeth[random.NextInt(0, sizeof(pMeth) / sizeof(ProportionalMethod))]);
			m_chambers[i]->SetProportionalSeats(m_chambers[i]->GetSeatCount());
		} else if (electSys == ElectoralSystem::ES_FPP_P_MIX) {
			ProportionalMethod pMeth[] = { ProportionalMethod::PM_DHONDT, ProportionalMethod::PM_IMPERIALY };
			m_chambers[i]->SetProportionalSystem(pMeth[random.NextInt(0, sizeof(pMeth) / sizeof(ProportionalMethod))]);
			m_chambers[i]->SetProportionalSeats((unsigned short)(m_chambers[i]->GetSeatCount() * random.NextFloat(0.5f)));
		} else {
			m_chambers[i]->SetProportionalSystem(ProportionalMethod::PM_NONE);
			m_chambers[i]->SetProportionalSeats(0);
		}

	}

	GenerateChamberPowers(pCountry, random);

}

void Legislature::GenerateChamberPowers(Country* pCountry, Random random) {

	// If bicameral
	if (m_isBicameral) {

		LegislativeChamber::ChamberPowers firstChamber; // First, lower chamber
		LegislativeChamber::ChamberPowers secondChamber; // second, upper chamber

		// Democracies elect their government as well
		if (pCountry->GetGovernment()->GetGovernmentType() == GovernmentType::Democracy) {
			
			// Government electability powers
			firstChamber.canElectGovernment = secondChamber.canElectGovernment = false; // Democracies elect their government, not the legislature
			
			// Government removal powers (eg. remove ministers, head of gov, head of state)
			firstChamber.canRemoveGovernment = random.NextBool(0.15f);
			secondChamber.canRemoveGovernment = (firstChamber.canRemoveGovernment) ? false : true;

			// Appointmental approvement (eg. approve ministers, judges etc.)
			firstChamber.canApproveAppointments = random.NextBool(0.15f);
			secondChamber.canApproveAppointments = (firstChamber.canApproveAppointments) ? false : true;

			// Law proposal powers
			firstChamber.canProposeLaws = random.NextBool(0.75f);
			secondChamber.canProposeLaws = (firstChamber.canProposeLaws) ? random.NextBool(0.25f) : random.NextBool(0.75f);

			// Law repeal powers
			firstChamber.canRepealLaws = random.NextBool(0.75f);
			secondChamber.canRepealLaws = (firstChamber.canRepealLaws) ? random.NextBool(0.25f) : random.NextBool(0.75f);

			// Law rejection powers
			firstChamber.canRejectLaws = secondChamber.canRejectLaws = true;

			// Law approval powers
			firstChamber.canApproveLaws = true;
			secondChamber.canApproveLaws = true;

		} else {

			bool isConsultingLegislature = random.NextBool(0.6f);

			if (isConsultingLegislature) {

				// Government electability powers
				firstChamber.canElectGovernment = secondChamber.canElectGovernment = false;

				// Government removal powers (eg. remove ministers, head of gov, head of state)
				firstChamber.canRemoveGovernment = false;
				secondChamber.canRemoveGovernment = false;

				// Appointmental approvement (eg. approve ministers, judges etc.)
				firstChamber.canApproveAppointments = false;
				secondChamber.canApproveAppointments = random.NextBool(0.5f);

				// Law proposal powers
				firstChamber.canProposeLaws = false;
				secondChamber.canProposeLaws = random.NextBool(0.15f);

				// Law repeal powers
				firstChamber.canRepealLaws = false;
				secondChamber.canRepealLaws = false;

				// Law rejection powers
				firstChamber.canRejectLaws = secondChamber.canRejectLaws = false;

				// Law approval powers
				firstChamber.canApproveLaws = true;
				secondChamber.canApproveLaws = true;

			} else {

				// Government electability powers
				firstChamber.canElectGovernment = random.NextBool(0.25f);
				secondChamber.canElectGovernment = false;

				// Government removal powers (eg. remove ministers, head of gov, head of state)
				firstChamber.canRemoveGovernment = firstChamber.canElectGovernment;
				secondChamber.canRemoveGovernment = false;

				// Appointmental approvement (eg. approve ministers, judges etc.)
				firstChamber.canApproveAppointments = firstChamber.canElectGovernment;
				secondChamber.canApproveAppointments = (firstChamber.canApproveAppointments) ? false : true;

				// Law proposal powers
				firstChamber.canProposeLaws = random.NextBool(0.75f);
				secondChamber.canProposeLaws = (firstChamber.canProposeLaws) ? random.NextBool(0.25f) : random.NextBool(0.75f);

				// Law repeal powers
				firstChamber.canRepealLaws = random.NextBool(0.75f);
				secondChamber.canRepealLaws = (firstChamber.canRepealLaws) ? random.NextBool(0.25f) : random.NextBool(0.75f);

				// Law rejection powers
				firstChamber.canRejectLaws = secondChamber.canRejectLaws = true;

				// Law approval powers
				firstChamber.canApproveLaws = true;
				secondChamber.canApproveLaws = true;

			}

		}

		// Assign chabmer powers
		m_chambers[0]->SetPowers(firstChamber);
		m_chambers[1]->SetPowers(secondChamber);

	} else {

		LegislativeChamber::ChamberPowers chamber;

		// Democracies elect their government as well
		if (pCountry->GetGovernment()->GetGovernmentType() == GovernmentType::Democracy) {

			// Government electability powers
			chamber.canElectGovernment = false;

			// Government removal powers (eg. remove ministers, head of gov, head of state)
			chamber.canRemoveGovernment = random.NextBool(0.15f);

			// Appointmental approvement (eg. approve ministers, judges etc.)
			chamber.canApproveAppointments = random.NextBool(0.15f);

			// Law proposal powers
			chamber.canProposeLaws = random.NextBool(0.75f);

			// Law repeal powers
			chamber.canRepealLaws = random.NextBool(0.75f);

			// Law rejection powers
			chamber.canRejectLaws = true;

			// Law approval powers
			chamber.canApproveLaws = true;

		} else {

			bool isConsultingLegislature = random.NextBool(0.6f);

			if (isConsultingLegislature) {

				// Government electability powers
				chamber.canElectGovernment = false;

				// Government removal powers (eg. remove ministers, head of gov, head of state)
				chamber.canRemoveGovernment = false;

				// Appointmental approvement (eg. approve ministers, judges etc.)
				chamber.canApproveAppointments = false;

				// Law proposal powers
				chamber.canProposeLaws = false;

				// Law repeal powers
				chamber.canRepealLaws = false;

				// Law rejection powers
				chamber.canRejectLaws = false;

				// Law approval powers
				chamber.canApproveLaws = true;

			} else {

				// Government electability powers
				chamber.canElectGovernment = random.NextBool(0.15f);

				// Government removal powers (eg. remove ministers, head of gov, head of state)
				chamber.canRemoveGovernment = random.NextBool(0.15f);

				// Appointmental approvement (eg. approve ministers, judges etc.)
				chamber.canApproveAppointments = random.NextBool(0.15f);

				// Law proposal powers
				chamber.canProposeLaws = random.NextBool(0.75f);

				// Law repeal powers
				chamber.canRepealLaws = chamber.canProposeLaws;

				// Law rejection powers
				chamber.canRejectLaws = random.NextBool(0.75f);

				// Law approval powers
				chamber.canApproveLaws = true;

			}

		}

		m_chambers[0]->SetPowers(chamber);

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
				if (pWorld->GetDate().isLaterOrSameThan(pSeat->GetElectDate().addYears(pChamber->GetTermLength()))) {

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

	// Push national news
	m_targetCountry->GetNewspaper()->LegislatureElectionResults(pWorld->GetDate(), electionResult);

	// Get the chamber powers
	LegislativeChamber::ChamberPowers* powers = pChamber->GetPowers();

	// Does the chamber elect a government?
	if (powers->canElectGovernment) {

		// Create new government from chamber election results
		m_targetCountry->GetGovernment()->NewGovernment(pChamber, pWorld->GetDate());

		// Add event of government forming
		pWorld->GetHistory()->AddEvent(pWorld->GetDate(), m_targetCountry, EVENT_TYPE::ELECTION_LEGISLATURE_GOVERNMENT, m_targetCountry->GetGovernment());

	} else {

		// Is the government a constitutional monarch AND is the chamber the first chamber
		if (m_targetCountry->GetGovernment()->GetGovernmentType() == GovernmentType::ConstitutionalMonarchism && m_chambers[0] == pChamber) {

			// A non-monarchist party has the majority
			if (pChamber->GetBiggestCoalitionLeader()->GetIdeology()->GetLibertarianStance() <= 0.0f) {

				// Appoint a new (hopefully more friendly) government
				m_targetCountry->GetGovernment()->AppointGovernment(pWorld->GetRandom(), pWorld->GetDate());

				// Add event to history
				pWorld->GetHistory()->AddEvent(pWorld->GetDate(), m_targetCountry, EVENT_TYPE::APPOINT_GOVERNMENT, m_targetCountry->GetGovernment());

			}

		}

	}

}

bool Legislature::HasPowerToElectGovernment() {
	if (m_isBicameral) {
		return m_chambers[0]->GetPowers()->canElectGovernment || m_chambers[1]->GetPowers()->canElectGovernment;
	} else {
		return m_chambers[0]->GetPowers()->canElectGovernment;
	}
}

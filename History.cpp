#include "History.h"
#include "Country.h"
#include "StringUtilities.h"
#include "LegislativeChamber.h"
#include <fstream>
#include <ostream>
#include <sstream>

History::History() {

}

void History::AddEvent(TimeDate date, Country* pCountry, EVENT_TYPE type, void* dat) {

	Event event;
	event.date = date;
	event.type = type;
	event.country = pCountry->GetName();

	if (type == EVENT_TYPE::ELECTION_LEGISLATURE) {
		CreateLegislatureElectionEvent(event, dat);
	} else if (type == EVENT_TYPE::APPOINT_GOVERNMENT || type == EVENT_TYPE::ELECTION_LEGISLATURE_GOVERNMENT || type == EVENT_TYPE::ELECTED_GOVERNMENT) {
		CreateGovernmentFormEvent(event, dat, pCountry);
	}

	m_events.push_back(event);

}

void History::CreateLegislatureElectionEvent(Event& event, void* dat) {

	auto results = (LegislatureElectionResult*)dat;

	std::stringstream ss;

	if (results->isMidTerms) {
		ss << "Midterm elections to the " << results->chamberName << ".\n";
	} else {
		ss << "Elections to the " << results->chamberName << ".\n";
	}

	ss << "\t" << results->seatUpForGrabs << " seats up for grabs of " << results->seatTotal << " seats (" << results->seatMajority << " for majority).";
	ss << " Using the " << ElectoralSystemNames[(int)results->electoralMethod];

	if (results->electoralMethod == ElectoralSystem::ES_PROPORTIONAL) {
		ss << " (" << ProportionalMethodNames[(int)results->proportionalMethod] << ")";
	}

	ss << " system.\n";

	if (results->runoffs != -1) {
		ss << "\tA total of " << results->runoffs << " runoff-elections took place during this election.\n";
	}

	for (auto seat : results->seats) {

		ss << "\t" << seat.first->GetName() << ", " << seat.first->GetShort() << ": " << seat.second << " seats (";

		if (results->gains[seat.first] >= 0) {
			ss << "+" << results->gains[seat.first] << ")";
		} else {
			ss << results->gains[seat.first] <<  ")";
		}

		ss << " and gained " << to_string(results->voteShare[seat.first] * 100.0f, 2) << "% of the vote\n";

	}

	ss << "\nA total of " << to_quantity(results->totalVotes) << " votes were cast with a turnout of " << to_string(results->turnout * 100.0f, 2) << "%";

	event.message = ss.str();

}

void History::CreateGovernmentFormEvent(Event& event, void* dat, Country* pCountry) {

	std::stringstream ss;

	if (event.type == EVENT_TYPE::APPOINT_GOVERNMENT) {

		// Government
		Government* gov = (Government*)dat;

		// Get the head of government
		Politician* primeminister = gov->GetHeadOfGovernment();

		ss << "A government has been appointed by " << pCountry->GetHeadOfState()->GetTitle() << ".\n";
		ss << "\tThe government will be lead by: " + primeminister->GetTitle();
	
		// Make sure the PM has a party to represent
		if (primeminister->GetParty() != NULL) {
			ss << ", representing the '" << primeminister->GetParty()->GetName() << "'.\n";
		} else {
			ss << " (NI).\n";
		}

		ss << "\tThe new government consists of " << gov->GetMinistryCount() << " ministries.";

	} else if (event.type == EVENT_TYPE::ELECTED_GOVERNMENT) {

		// Government
		Government* gov = pCountry->GetGovernment();

		// Get results
		PresidentialElectionResults* results = (PresidentialElectionResults*)dat;

		ss << "A new government has been democratically elected.\n";

		if (gov->GetElectionSystem() == GovernmentElectoralSystem::WinnerTakesAll || gov->GetElectionSystem() == GovernmentElectoralSystem::TwoRoundSystem) {

			ss << "Using the " << ((gov->GetElectionSystem() == GovernmentElectoralSystem::TwoRoundSystem) ? "Two-Round" : "Winner Takes All") << " method, ";
			ss << gov->GetHeadOfGovernment()->GetTitle() << " has been elected with " << to_string(results->candidateVoteShare[results->pWinner] * 100.0, 2) << "% of the vote.\n";

			ss << "\tElection Results:\n";

			for (auto c : results->candidateVoteShare) {
				ss << "\t\t" << c.first->GetFullName() << " (" << c.first->GetParty()->GetShort() << ") received " << to_string(c.second * 100.0, 2) << "%.\n";
			}

			ss << "\tVoter turnout: " << to_string(results->turnout * 100.0, 2) << "%.\n";

		} else {

			ss << "Using the Electoral College election system, with " << results->electoralCollegeElectors << " electors, ";
			ss << gov->GetHeadOfGovernment()->GetTitle() << " has been elected with " << results->candidateElectoralVotes[results->pWinner] << " electoral votes.\n";

			ss << "\tElection Results:\n";

			for (auto c : results->candidateElectoralVotes) {
				ss << "\t\t" << c.first->GetFullName() << " (" << c.first->GetParty()->GetShort() << ") received " << c.second << " electoral college votes.\n";
			}

			ss << "\tVoter turnout: " << to_string(results->turnout * 100.0, 2) << "%.\n";

		}

		ss << "\tThe new government consists of " << gov->GetMinistryCount() << " ministries.";

	} else if (event.type == EVENT_TYPE::ELECTION_LEGISLATURE_GOVERNMENT) {

		// Government
		Government* gov = (Government*)dat;

		LegislativeCoalition* legCol = gov->GetCoalition();
		std::string govType = (legCol->type == CoalitionType::Majority) ? "majority" : "minority";

		ss << "Following recent elections a new " << govType << " government has been formed by " << gov->GetHeadOfGovernment()->GetTitle() << ".\n";
		ss << "\tThe new government consists of " << gov->GetMinistryCount() << " ministries.\n";

		ss << "\tThe government is lead by the '" << legCol->leader->GetName() << "' party.\n";
		ss << "\tA total of " << legCol->seats << " seats in parliament backs the government.\n";

		if (legCol->partners.size() > 0) {
			ss << "Where, " << legCol->seatsSupplied << " seats are supplied by: ";
			size_t comma = 1;
			for (auto supply : legCol->partners) {
				ss << "'" << supply->GetName() << "'";
				if (comma <= legCol->partners.size() - 1) {
					ss << ", ";
				} else {
					ss << " and ";
				}
				comma++;
			}
		}

	}

	event.message = ss.str();

}

void History::LogDecade(std::string name, int from, int to) {

	std::ofstream fOutStream = std::ofstream(name + "_" + std::to_string(from) + "-" + std::to_string(to) + ".txt");

	for (auto e : m_events) {

		fOutStream << e.date.toString() << ", " << e.country << std::endl;
		fOutStream << e.message << std::endl;

		fOutStream << std::endl;

	}

	fOutStream.close();

	m_events.clear();

}

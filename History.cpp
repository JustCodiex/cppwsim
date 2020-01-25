#include "History.h"
#include "Country.h"
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

	auto results = (LegislativeChamber::LegislatureElectionResult*)dat;

	std::stringstream ss;

	if (results->isMidTerms) {
		ss << "Midterm elections to the " << results->chamberName << ".\n";
	} else {
		ss << "Elections to the " << results->chamberName << ".\n";
	}

	ss << "\t" << results->seatUpForGrabs << " seats up for grabs of " << results->seatTotal << " seats (" << results->seatMajority << " for majority).\n";

	if (results->runoffs != -1) {
		ss << "\tA total of " << results->runoffs << " runoff-elections took place.\n";
	}

	for (auto seat : results->seats) {

		ss << "\t" << seat.first->GetName() << ", " << seat.first->GetShort() << ": " << seat.second << " seats (";

		if (results->gains[seat.first] >= 0) {
			ss << "+" << results->gains[seat.first] << ")";
		} else {
			ss << results->gains[seat.first] <<  ")";
		}

		ss << " and gained " << (results->voteShare[seat.first] * 100.0f) << "% of the vote\n";

	}

	ss << "\nA total of " << results->totalVotes << " votes were cast with a turnout of " << (results->turnout * 100.0f) << "%";

	event.message = ss.str();

}

void History::CreateGovernmentFormEvent(Event& event, void* dat, Country* pCountry) {

	Government* gov = (Government*)dat;

	std::stringstream ss;

	if (event.type == EVENT_TYPE::APPOINT_GOVERNMENT) {

		ss << "New government appointed by " << pCountry->GetHeadOfState()->GetFullName() << ".\n";
		ss << "\tThe government will be lead by: " + gov->GetHeadOfGovernment()->GetFullName() << ", representing the '" << gov->GetHeadOfGovernment()->GetParty() << "' party.\n";
		ss << "\tThe new government consists of " << gov->GetMinistryCount() << " ministries.";

	} else if (event.type == EVENT_TYPE::ELECTED_GOVERNMENT) {



	} else if (event.type == EVENT_TYPE::ELECTION_LEGISLATURE_GOVERNMENT) {

		LegislativeCoalition* legCol = gov->GetCoalition();
		std::string govType = (legCol->type == CoalitionType::Majority) ? "majority" : "minority";

		ss << "Following recent elections a new " << govType << " government has been formed by " << gov->GetHeadOfGovernment()->GetFullName() << ".\n";
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

#include "History.h"
#include "Country.h"
#include "LegislativeChamber.h"
#include <fstream>
#include <ostream>

History::History() {

}

void History::AddEvent(TimeDate date, Country* pCountry, EVENT_TYPE type, void* dat) {

	Event event;
	event.date = date;
	event.type = type;
	event.country = pCountry->GetName();

	if (type == EVENT_TYPE::ELECTION_LEGISLATURE) {
		CreateLegislatureElectionEvent(event, dat);
	}

	m_events.push_back(event);

}

void History::CreateLegislatureElectionEvent(Event& event, void* dat) {

	auto results = (LegislativeChamber::LegislatureElectionResult*)dat;

	std::string msg;

	if (results->isMidTerms) {
		msg = "Midterm elections to the " + results->chamberName + ".\n";
	} else {
		msg = "Elections to the " + results->chamberName + ".\n";
	}

	for (auto seat : results->seats) {

		msg += "\t" + seat.first->GetName() + ": " + std::to_string(seat.second) + " seats (";

		if (results->gains[seat.first] >= 0) {
			msg += "+" + std::to_string(results->gains[seat.first]) + ")";
		} else {
			msg += std::to_string(results->gains[seat.first]) + ")";
		}

		msg += " and gained " + std::to_string(results->voteShare[seat.first] * 100.0f) + "% of the vote\n";

	}

	msg += "\nA total of " + std::to_string(results->totalVotes) + " votes were cast with a turnout of " + std::to_string(results->turnout * 100.0f) + "%";

	event.message = msg;

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

#pragma once
#include "stdlib.h"

class Country;

enum class EVENT_TYPE {
	ELECTION_LEGISLATURE,
	ELECTION_LEGISLATURE_GOVERNMENT, // A government is formed based on legislature results
	APPOINT_GOVERNMENT, // A government is appointed
	ELECTED_GOVERNMENT, // A government is elected (democracies)
};

class History {

public:

	struct Event {
		EVENT_TYPE type;
		TimeDate date;
		std::string country;
		std::string message;
	};

public:

	History();

	void AddEvent(TimeDate date, Country* pCountry, EVENT_TYPE type, void* dat);

	void LogDecade(std::string name, int from, int to);

private:

	void CreateLegislatureElectionEvent(Event& event, void* dat);

	void CreateGovernmentFormEvent(Event& event, void* dat, Country* pCountry);

private:

	std::vector<Event> m_events;

};

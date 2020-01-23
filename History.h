#pragma once
#include <vector>
#include "TimeDate.h"

class Country;

enum class EVENT_TYPE {
	ELECTION_LEGISLATURE,
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

private:

	std::vector<Event> m_events;

};

#pragma once
#include "TimeDate.h"

class Country;

enum class EVENT_TYPE {
	ELECTION_LEGISLATURE,
};

class History {

public:

	History();

	void AddEvent(TimeDate date, Country* pCountry, EVENT_TYPE type, void* dat);

	void LogDecade(int from, int to);

private:



};

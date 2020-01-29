#pragma once
#include "City.h"

// An administrative level in the organization of a country.
// Either a state or a region.
class CountryAdministrationLevel {

public:

	// Get the administrative level
	virtual int GetAdminLevel() = 0;

	// Get the size of the population
	virtual unsigned int GetPopulationSize() = 0;

	// Get the cities under the administration level
	virtual std::vector<City*> GetCities() = 0;

};

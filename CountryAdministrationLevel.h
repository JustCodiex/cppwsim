#pragma once
#include "City.h"

// An administrative level in the organization of a country.
// Either a state or a region.
class CountryAdministrationLevel : public EconomyLevel {

public:

	// Get the administrative level
	virtual int GetAdminLevel() = 0;

	// Get the size of the population
	virtual PopSize GetPopulationSize() = 0;

	// Get the cities under the administration level
	virtual std::vector<City*> GetCities() = 0;

	// Get the economy level of the country level
	int GetEconomyLevel() override final { return this->GetAdminLevel(); };

};

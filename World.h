#pragma once
#include <string>
#include <vector>
#include "Random.h"

class Country;

class World {

public:

	World(std::string name);

	void Generate(Random random);

	std::string GetName() { return m_worldName; }
	std::vector<Country*> GetCountries() { return m_countries; }

	Country* NewCountry(Random random);

private:

	std::vector<Country*> m_countries;
	std::string m_worldName;

};


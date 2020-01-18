#pragma once
#include <string>
#include <vector>
#include "Random.h"
#include "TimeDate.h"
#include "WorldEconomy.h"

class Country;

class World {

public:

	World(std::string name);

	void Generate(Random random);

	std::string GetName() { return m_worldName; }
	std::vector<Country*> GetCountries() { return m_countries; }

	Country* NewCountry(Random random);
	Country* GetCountry(int index) { return m_countries[index]; }

	void UpdateWorld(TimeDate date);

	WorldEconomy* GetWorldEconomy() { return &m_worldEconomy; }

	TimeDate GetDate() { return m_currentDate; }

private:


	TimeDate m_currentDate;

	WorldEconomy m_worldEconomy;

	std::vector<Country*> m_countries;
	std::string m_worldName;

};

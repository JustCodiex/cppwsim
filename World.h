#pragma once
#include <string>
#include <vector>
#include "Random.h"
#include "History.h"
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

	History* GetHistory() { return m_worldHistory; }

	Random GetRandom() { return m_worldRandom; }

private:


	TimeDate m_currentDate;
	TimeDate m_lastDate;

	WorldEconomy m_worldEconomy;

	Random m_worldRandom;

	History* m_worldHistory;

	std::vector<Country*> m_countries;
	std::string m_worldName;

};

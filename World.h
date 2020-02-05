#pragma once
#include "stdlib.h"
#include "History.h"
#include "Economy.h"
#include "Market.h"

class Country;

class World : public EconomyLevel {

public:

	World(std::string name);

	void Generate(Random random);

	std::string GetName() { return m_worldName; }
	std::vector<Country*> GetCountries() { return m_countries; }

	Country* NewCountry(Random random);
	Country* GetCountry(int index) { return m_countries[index]; }

	void UpdateWorld(TimeDate date);

	Economy* GetWorldEconomy() { return &m_worldEconomy; }
	Market* GetGlobalEconomy() { return &m_globalMarket; }

	TimeDate GetDate() { return m_currentDate; }

	History* GetHistory() { return m_worldHistory; }

	Random GetRandom() { return m_worldRandom; }

	int GetEconomyLevel() override { return 4; }

private:


	TimeDate m_currentDate;
	TimeDate m_lastDate;

	Economy m_worldEconomy;
	Market m_globalMarket;

	Random m_worldRandom;

	History* m_worldHistory;

	std::vector<Country*> m_countries;
	std::string m_worldName;

};

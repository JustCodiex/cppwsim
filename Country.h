#pragma once
#include <string>
#include "Random.h"
#include "Government.h"
#include "CountryEconomy.h"

class World;

enum class CountryType {
	Kingdom,
	Republic,
	Federation,
};

class Country {

public:

	Country(std::string name);

	void GenerateCountry(Random random);

	std::string GetName() { return m_countryName; }

	void UpdateCountry(World* pWorld);

	Government* GetGovernment() { return &m_countryGovernment; }

	CountryEconomy* GetEconomy() { return &m_countryEconomy; }

private:

	void UpdateEconomy(World* pWorld);

private:

	std::string m_countryName;

	Government m_countryGovernment;

	CountryEconomy m_countryEconomy;

};

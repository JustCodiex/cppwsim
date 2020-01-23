#include "World.h"
#include "Country.h"
#include "name_countries.h"

World::World(std::string worldname) {
	
	m_worldName = worldname;
	m_worldHistory = new History;

}

void World::Generate(Random random) {

	// Get a random amount of countries
	int countryCount = random.NextInt(3, 7);

	// For each country
	for (int i = 0; i < countryCount; i++) {

		// Create country
		Country* pCountry = this->NewCountry(random);

		// Add country to country list
		m_countries.push_back(pCountry);

	}

}

Country* World::NewCountry(Random random) {

	// Create a new country
	Country* country = new Country(nextCountryName(random));

	// Generate country
	country->GenerateCountry(random);

	// Return newly generated country
	return country;

}

void World::UpdateWorld(TimeDate date) {

	// Update time
	m_currentDate = date;

	// For each Country in the world, update
	for (auto* country : m_countries) {

		country->UpdateCountry(this);

	}

	int lastSaveDate = m_lastDate.getTotalDays();

	if (lastSaveDate == 1) {
		m_lastDate = m_currentDate;
	} else {
		if (m_currentDate.getTotalDays() - lastSaveDate > TimeDate::Decade.getTotalDays()) {
			m_worldHistory->LogDecade(m_lastDate.year, m_currentDate.year);
			m_lastDate = m_currentDate;
		}
	}

}

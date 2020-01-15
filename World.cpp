#include "World.h"
#include "Country.h"
#include "name_countries.h"

World::World(std::string worldname) {
	
	m_worldName = worldname;

}

void World::Generate(Random random) {

	int countryCount = random.NextInt(5, 16);

	for (int i = 0; i < countryCount; i++) {

		Country* pCountry = this->NewCountry(random);

		m_countries.push_back(pCountry);

	}

}

Country* World::NewCountry(Random random) {

	Country* country = new Country(nextCountryName(random));

	return country;

}

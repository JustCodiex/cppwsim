#include "Region.h"

Region::Region() {
	m_regionalCouncil = 0;
}

void Region::Generate(bool isPartOfFederation, Random random) {

	// Generate cities
	GenerateCities(random);

	// If not part of a federal country
	if (!isPartOfFederation) {

		// Create regional council
		m_regionalCouncil = new Legislature;

	}

}

void Region::GenerateCities(Random random) {

	int cityCount = random.NextInt(3, 21);

	if (cityCount > 12) { // lots of small cities + (cityCount * 0.2) big cities

		int smallCities = (int)(cityCount * 0.2);
		int bigCities = cityCount - smallCities;

		for (int i = 0; i < smallCities; i++) {
			m_cities.push_back(new City(City::Size::CITYSIZE_SMALL, random));
		}

		for (int i = 0; i < bigCities; i++) {
			m_cities.push_back(new City(City::Size::CITYSIZE_LARGE, random));
		}

	} else { // 1 BIG city, the rest is small cities

		int majorCity = 1;
		int minorCities = cityCount - 1;

		for (int i = 0; i < minorCities; i++) {
			m_cities.push_back(new City(City::Size::CITYSIZE_SMALL, random));
		}

		m_cities.push_back(new City(City::Size::CITYSIZE_METROPOLIS, random));

	}

}

unsigned int Region::GetPopulationSize() {
	unsigned int count = 0;
	for (City*& city : m_cities) {
		count += city->GetPopulationSize();
	}
	return count;
}

int Region::GetCityCount() {
	return (int)m_cities.size();
}

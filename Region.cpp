#include "Region.h"
#include "State.h"
#include "Country.h"

Region::Region() {
	
	m_state = 0;
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

City* Region::NewCity(int size, Random random) {

	// The size of the city
	City::Size sz = (City::Size)size;

	// Create new city
	City* pCity = new City(sz, random);

	// Make sure we managed to create it
	if (!pCity) {
		throw std::exception("Failed to create city!");
	}

	// Set region and name
	pCity->SetRegionAndName(random, this);

	// Generate the economy
	pCity->GenerateEconomy(random);

	// Get the market of the country
	pCity->GetMarket()->SetNextMarket(m_state->GetCountry()->GetMarket());

	// Return city
	return pCity;

}

void Region::GenerateCities(Random random) {

	int cityCount = random.NextInt(3, 21);

	if (cityCount > 12) { // lots of small cities + (cityCount * 0.4) big cities

		int smallCities = (int)(cityCount * 0.6);
		int bigCities = cityCount - smallCities;

		for (int i = 0; i < smallCities; i++) {
			m_cities.push_back(this->NewCity(0, random));
		}

		for (int i = 0; i < bigCities; i++) {
			m_cities.push_back(this->NewCity(1, random));
		}

	} else { // 1 BIG city, the rest is small cities

		int majorCity = 1;
		int minorCities = cityCount - 1;

		std::vector<Weight<City::Size>> citySizes = {
			Weight(0.4f, City::Size::CITYSIZE_LARGE),
			Weight(0.6f, City::Size::CITYSIZE_SMALL),
		};

		for (int i = 0; i < minorCities; i++) {
			m_cities.push_back(this->NewCity((int)random.Select(citySizes), random));
		}

		m_cities.push_back(this->NewCity(2, random));

	}

}

PopSize Region::GetPopulationSize() {
	PopSize count = 0;
	for (auto city : m_cities) {
		count += city->GetPopulationSize();
	}
	return count;
}

int Region::GetCityCount() {
	return (int)m_cities.size();
}

void Region::UpdateEconomy(int days) {

	for (auto city : m_cities) {
		city->UpdateEconomy(days);
	}

}

void Region::UpdateDemographics() {

	for (auto city : m_cities) {
		city->UpdateDemographics();
	}

}

City* Region::GetCityByName(std::string name) {

	for (auto city : m_cities) {
		if (city->GetName() == name) {
			return city;
		}
	}

	return NULL;

}

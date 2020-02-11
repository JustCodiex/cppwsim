#include "City.h"
#include "Region.h"
#include "State.h"
#include "Company.h"
#include "Country.h"
#include "CountryProfile.h"
#include "name_cities.h"

City::City() {
	
	m_region = 0;
	m_population = 0;
	m_isCoastal = false;
	m_size = Size::CITYSIZE_SMALL;
	m_cityStats = Statistics();
	m_name = "Unnamed City";
	m_cityEconomy = Economy(this);
	m_localMarket = Market(this);

}

City::City(Size size, Random random) {

	// Set city economy
	m_cityEconomy = Economy(this);
	m_localMarket = Market(this);

	// Store size of city
	m_size = size;

	if (size == Size::CITYSIZE_LARGE) {

		m_population = random.NextInt(11000, 80000);

	} else if (size == Size::CITYSIZE_METROPOLIS) {

		m_population = random.NextInt(100000, 400000);

	} else { // Small

		m_population = random.NextInt(700, 10000);

	}

	// Generate demographics
	GenerateDemographics(random);

	// Gnerate city statistics
	GenerateStatistics(random);

}

void City::SetRegionAndName(Random random, Region* pRegion) {

	// Assign region
	m_region = pRegion;

	// While name is unique in region
	while (true) {

		// Get "random" city name
		std::string randCityName = nextCityName(random);

		// Try and fetch city by name
		if (pRegion->GetCityByName(randCityName) == NULL) {

			// No city found, so it's available
			m_name = randCityName;
			break;

		} else {

			// Try and put "new" on and see if it's available then
			randCityName = "New " + randCityName;

			// Make sure it's available
			if (pRegion->GetCityByName(randCityName) == NULL) {

				// No city found, so it's available
				m_name = randCityName;
				break;

			}

		}

	}

}

void City::GenerateDemographics(Random random) {

	// Percentage of each social class making up the city
	float socialClasses[(int)SOCIAL_CLASS::CLASS_COUNT];
	socialClasses[(int)SOCIAL_CLASS::LOWER_CLASS] = 0.4f - random.NextFloat(0.05f);
	socialClasses[(int)SOCIAL_CLASS::LOWER_MIDDLE_CLASS] = 0.35f - random.NextFloat(0.1f);
	socialClasses[(int)SOCIAL_CLASS::MIDDLE_CLASS] = 0.1f - random.NextFloat(0.05f);
	socialClasses[(int)SOCIAL_CLASS::UPPER_MIDDLE_CLASS] = 0.05f - random.NextFloat(0.025f);
	socialClasses[(int)SOCIAL_CLASS::UPPER_CLASS] = 0.1f - random.NextFloat(0.05f);

	// Make social class chart (any missing numbers will be generated fixed here)
	m_socialClass = FixedChart(socialClasses, (int)SOCIAL_CLASS::CLASS_COUNT);

}

void City::GenerateStatistics(Random random) {

	// The birthrate of the city
	m_cityStats.birthRate = random.NextFloat(0.01f, 0.025f);

}

void City::GenerateEconomy(Random random) {

	int agriculturalMin, agriculturalMax;

	if (m_size == City::Size::CITYSIZE_SMALL) { // Much Agriculture, Low industry

		agriculturalMin = 5;
		agriculturalMax = 10;

	} else if (m_size == City::Size::CITYSIZE_LARGE) { // Some industry, no agriculture

		agriculturalMin = 0;
		agriculturalMax = 5;

	} else { // Metropolis

		agriculturalMin = agriculturalMax = 0;

	}

	// Create agriculture
	CreateAgriculture(random, agriculturalMin, agriculturalMax);

	// Create foods
	CreateFoodCompanies(random, agriculturalMin / 3, agriculturalMax / 3);

}

void City::CreateAgriculture(Random random, int min, int max) {

	// Make sure there's something to do
	if (min == max && min == 0) {
		return;
	}

	// Find out how many agricultural companies to make
	int agricultural = random.NextInt(min, max);

	// Create these agricultural companies
	for (int i = 0; i < agricultural; i++) {

		// Set output type
		Company* farm = new Company(CompanySector::Primary, CompanyOwnership::Private);
		farm->SetOutputType(random.NextBool(0.35f) ? MarketProduct::Animals : MarketProduct::Produce);
		farm->EnterMarket(&m_localMarket);

		// Register company
		m_region->GetState()->GetCountry()->RegisterCompany(farm);

		// Register company as local
		m_localCompanies.push_back(farm);

	}

}

void City::CreateFoodCompanies(Random random, int min, int max) {

	// Make sure there's something to do
	if (min == max && min == 0) {
		return;
	}

	// Find out how many agricultural companies to make
	int foods = random.NextInt(min, max);

	// Create these food companies
	for (int i = 0; i < foods; i++) {

		// Set output type
		Company* food = new Company(CompanySector::Secondary, CompanyOwnership::Private);
		food->SetInputType(random.NextBool(0.35f) ? MarketProduct::Animals : MarketProduct::Produce);
		food->SetOutputType(MarketProduct::Food);
		food->EnterMarket(&m_localMarket);

		// Register company
		m_region->GetState()->GetCountry()->RegisterCompany(food);

		// Register company as local
		m_localCompanies.push_back(food);

	}

}

PopSize City::GetVoterCount(CountryProfile* pProfile) {

	PopSize votPop = 0;

	for (int i = 0; i < (int)SOCIAL_CLASS::CLASS_COUNT; i++) {
		votPop += this->GetSocialClass((SOCIAL_CLASS)i, pProfile, true);
	}

	return votPop;

}

PopSize City::GetSocialClass(SOCIAL_CLASS socialClass, CountryProfile* pProfile, bool voters) {

	PopSize pop = (PopSize)((double)m_socialClass.At((int)socialClass) * m_population);

	if (!voters) {
		return pop;
	}

	if (!pProfile->voting_female_suffrage) {
		pop = (PopSize)(pop * 0.51);
	}

	if (pProfile->voting_require_land_and_business && socialClass == SOCIAL_CLASS::UPPER_CLASS) {
		return pop;
	} else if (!pProfile->voting_require_land_and_business) {
		if (pProfile->voting_require_land && socialClass >= SOCIAL_CLASS::UPPER_MIDDLE_CLASS) {
			return pop;
		} else if (pProfile->voting_require_business && socialClass >= SOCIAL_CLASS::MIDDLE_CLASS) {
			return pop;
		} else {
			if ((pProfile->voting_require_business || pProfile->voting_require_land) && socialClass >= SOCIAL_CLASS::LOWER_MIDDLE_CLASS) {
				return (PopSize)(pop * 0.225);
			} else if (!pProfile->voting_require_business && !pProfile->voting_require_land) {
				return pop;
			}
		}
	}

	return 0;

}

void City::UpdateEconomy(int days) {

	// Update the local market
	m_localMarket.UpdateMarket();

	// Update the local companies
	for (Company* company : m_localCompanies) {

		// Update the company
		company->UpdateCompany(&m_localMarket);

	}

	// Tell the city economy to update itself
	m_cityEconomy.UpdateEconomy();

}

void City::UpdateDemographics() {

	// Update population size
	m_population += (PopSize)(m_population * (double)m_cityStats.birthRate);

}

std::string City::GetFullName() {

	return m_name + ", "; // TODO: return region, state, and country when that's been implemented

}

void City::UnregisterCompany(Company* pCompany) {
	auto itt = FIND_VECTOR_ELEMENT(m_localCompanies, pCompany);
	if (itt != m_localCompanies.end()) {
		m_localCompanies.erase(itt);
	}
}

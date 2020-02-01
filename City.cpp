#include "City.h"
#include "CountryProfile.h"

City::City() {
	
	m_population = 0;
	m_isCoastal = false;
	m_size = Size::CITYSIZE_SMALL;

}

City::City(Size size, Random random) {

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

void City::UpdateEconomy() {



}

void City::UpdateDemographics() {

	// Update population size
	m_population += (PopSize)(m_population * (double)m_cityStats.birthRate);

}

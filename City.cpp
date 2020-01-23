#include "City.h"
#include "CountryProfile.h"

City::City() {
	
	m_population = 0;
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

unsigned int City::GetVoterCount(CountryProfile* pProfile) {

	unsigned int votPop = 0;

	for (int i = 0; i < (int)SOCIAL_CLASS::CLASS_COUNT; i++) {
		votPop += this->GetSocialClass((SOCIAL_CLASS)i, pProfile, true);
	}

	return votPop;

}

unsigned int City::GetSocialClass(SOCIAL_CLASS socialClass, CountryProfile* pProfile, bool voters) {

	unsigned int pop = (unsigned int)(m_socialClass.At((int)socialClass) * m_population);

	if (!voters) {
		return pop;
	}

	if (!pProfile->voting_female_suffrage) {
		pop = (unsigned int)(pop * 0.51f);
	}

	if (pProfile->voting_require_land_and_business && socialClass == SOCIAL_CLASS::UPPER_CLASS) {
		return pop;
	} else if (!pProfile->voting_require_land_and_business) {
		if (pProfile->voting_require_land && socialClass >= SOCIAL_CLASS::UPPER_MIDDLE_CLASS) {
			return pop;
		} else if (pProfile->voting_require_business && socialClass >= SOCIAL_CLASS::MIDDLE_CLASS) {
			return pop;
		} else {
			if (pProfile->voting_require_business || pProfile->voting_require_land && socialClass >= SOCIAL_CLASS::LOWER_MIDDLE_CLASS) {
				return (unsigned int)(pop * 0.4f);
			} else if (!pProfile->voting_require_business && !pProfile->voting_require_land) {
				return pop;
			}
		}
	}

	return 0;

}
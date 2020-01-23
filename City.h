#pragma once
#include <vector>
#include "Random.h"
#include "Legislature.h"
#include "Government.h"
#include "CityDistrict.h"
#include "FixedChart.h"
#include "SocialClass.h"

struct CountryProfile;

class City {

public:
	enum class Size {
		CITYSIZE_SMALL,
		CITYSIZE_LARGE,
		CITYSIZE_METROPOLIS,
	};

public:

	City();
	City(Size size, Random random);

	unsigned int GetPopulationSize() { return m_population; }
	unsigned int GetVoterCount(CountryProfile* pProfile);

	unsigned int GetSocialClass(SOCIAL_CLASS socialClass, CountryProfile* pProfile, bool voters = false);

	FixedChart GetSocialClassChart() { return m_socialClass; }

	Size GetCitySize() { return m_size; }

	bool IsCoastal() { return m_isCoastal; }

	bool HasUniversity() { return false; }

private:

	void GenerateDemographics(Random random);

private:

	Size m_size;
	bool m_isCoastal;

	FixedChart m_socialClass;

	std::vector<CityDistrict> m_districts;
	unsigned int m_population;

};

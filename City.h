#pragma once
#include "stdlib.h"
#include "Legislature.h"
#include "Government.h"
#include "FixedChart.h"
#include "SocialClass.h"

class Region;
struct CountryProfile;

class City {

public:

	enum class Size {
		CITYSIZE_SMALL,
		CITYSIZE_LARGE,
		CITYSIZE_METROPOLIS,
	};

	struct Statistics {
		double birthRate;
		Statistics() {
			birthRate = 0.0;
		}
	};

public:

	City();
	City(Size size, Random random);

	void SetRegionAndName(Random random, Region* pRegion);
	Region* GetRegion() { return m_region; }

	std::string GetName() { return m_name; }
	std::string GetFullName();

	PopSize GetPopulationSize() { return m_population; }
	PopSize GetVoterCount(CountryProfile* pProfile);

	PopSize GetSocialClass(SOCIAL_CLASS socialClass, CountryProfile* pProfile, bool voters = false);

	FixedChart GetSocialClassChart() { return m_socialClass; }

	Size GetCitySize() { return m_size; }

	bool IsCoastal() { return m_isCoastal; }

	bool HasUniversity() { return false; }

	void UpdateEconomy();
	void UpdateDemographics();

	Statistics* GetCityStatistics() { return &m_cityStats; }

private:

	void GenerateDemographics(Random random);
	void GenerateStatistics(Random random);

private:

	std::string m_name;

	Region* m_region;

	Size m_size;
	bool m_isCoastal;

	FixedChart m_socialClass;

	PopSize m_population;

	Statistics m_cityStats;

};

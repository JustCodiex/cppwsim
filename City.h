#pragma once
#include "stdlib.h"
#include "Legislature.h"
#include "Government.h"
#include "FixedChart.h"
#include "SocialClass.h"
#include "Economy.h"
#include "EconomyLevel.h"
#include "Market.h"

class Region;
class Company;
struct CountryProfile;

class City : public EconomyLevel {

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

	void UpdateEconomy(int days);
	void UpdateDemographics();

	Statistics* GetCityStatistics() { return &m_cityStats; }

	Economy* GetEconomy() { return &m_cityEconomy; }
	Market* GetMarket() { return &m_localMarket; }

	int GetEconomyLevel() override { return 0; }

	void GenerateEconomy(Random random);

	void UnregisterCompany(Company* pCompany);

private:

	void GenerateDemographics(Random random);
	void GenerateStatistics(Random random);
	
	void CreateAgriculture(Random random, int min, int max);
	void CreateFoodCompanies(Random random, int min, int max);

private:

	std::string m_name;

	Region* m_region;

	Size m_size;
	bool m_isCoastal;

	FixedChart m_socialClass;

	PopSize m_population;

	Statistics m_cityStats;
	Economy m_cityEconomy;
	Market m_localMarket;

	std::vector<Company*> m_localCompanies;

	// Allow the city economy class to access privates
	friend class CityEconomy;

};

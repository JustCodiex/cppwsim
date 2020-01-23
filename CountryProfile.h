#pragma once
#include "Random.h"
#include <string>
#include <vector>

struct CountryProfile {

	float countryLibertarianModifier; // Libertarianism vs Authoritarianism [-1.0f to 1.0f]
	float countryCollectivismModifier; // Collectivism vs Individualism [-1.0f to 1.0f]

	float countryPoliticalApathy; // How interested the voters are in politics

	bool voting_female_suffrage;
	bool voting_require_land;
	bool voting_require_business;
	bool voting_require_land_and_business;

};

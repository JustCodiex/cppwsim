#pragma once
#include <vector>
#include <map>

class City;
class Ballot;
class Country;
class Politician;

struct CountryProfile;

enum class ElectionLevel {
	Referendum,
	Local,
	State,
	National,
};

struct ElectoralDistrictResult {
	std::map< Politician*, int> votes;
	std::map< Politician*, float> voteshare;
	unsigned int totalVotes;
	float turnout;
};

class ElectoralDistrict {

public:

	struct ElectoralCity {
		City* city;
		float partition;
		ElectoralCity() {
			city = 0;
			partition = 1.0f;
		}
	};

public:

	ElectoralDistrict(ElectionLevel lvl);

	void SetDistrict(City* pCity, float fSizePercentage);

	unsigned int GetVoterCount(CountryProfile* pProfile);

	Ballot* CreateBallot(Country* pCountry);

	ElectoralDistrictResult CastVotes(Ballot* pBallot, Country* pCountry);

	ElectoralDistrict* Split();

	std::vector< ElectoralCity> GetCities() { return m_cities; }

	static ElectoralDistrict* MergeAndDelete(ElectoralDistrict* pSmall, ElectoralDistrict* pAlmostSmall);
	
private:

	ElectionLevel m_electionLevel;
	std::vector< ElectoralCity> m_cities;

};

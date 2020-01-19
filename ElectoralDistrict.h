#pragma once
#include <vector>

class City;
class Ballot;
class Country;

enum class ElectionLevel {
	City,
	Regional,
	State,
	National,
};

struct ElectoralDistrictResult {

};

class ElectoralDistrict {

private:

	struct ElectoralCity {
		City* city;
		float partition;
	};

public:

	ElectoralDistrict(ElectionLevel lvl);

	void SetDistrict(City* pCity, float fSizePercentage);

	unsigned int GetVoterCount();

	Ballot* CreateBallot(Country* pCountry);

	ElectoralDistrictResult CastVotes(Ballot* pBallot, Country* pCountry);

	ElectoralDistrict* Split();

	static ElectoralDistrict* MergeAndDelete(ElectoralDistrict* pSmall, ElectoralDistrict* pAlmostSmall);
	
private:

	ElectionLevel m_electionLevel;
	std::vector< ElectoralCity> m_cities;

};

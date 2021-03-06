#pragma once
#include "stdlib.h"

class City;
class Ballot;
class Country;
class Politician;
class LegislativeSeat;

struct CountryProfile;

enum class ElectionLevel {
	Referendum,
	Local,
	State,
	National,
};

struct ElectoralDistrictResult {
	std::map< Politician*, PopSize> votes;
	std::map< Politician*, double> voteshare;
	PopSize totalVotes;
	double turnout;
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

	void SetDistrict(City* pCity, float fSizePercentage, CountryProfile* pProfile);

	void SetSeat(LegislativeSeat* pSeat) { m_seat = pSeat; }

	LegislativeSeat* GetSeat() { return m_seat; }

	PopSize GetVoterCount() { return m_voters; }

	Ballot* CreateBallot(Country* pCountry);

	ElectoralDistrictResult CastVotes(Ballot* pBallot, Country* pCountry);

	ElectoralDistrict* Split(CountryProfile* pProfile);

	std::vector< ElectoralCity> GetCities() { return m_cities; }

	static ElectoralDistrict* Merge(ElectoralDistrict* pSmall, ElectoralDistrict* pAlmostSmall, CountryProfile* pProfile);
	
private:

	void UpdateVoterCount(CountryProfile* pProfile);

private:

	PopSize m_voters;

	LegislativeSeat* m_seat;

	ElectionLevel m_electionLevel;
	std::vector< ElectoralCity> m_cities;

};

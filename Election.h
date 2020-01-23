#pragma once
#include <vector>
#include <map>

class City;
class Ballot;
class Country;
class Politician;
class PoliticalParty;
class ElectoralDistrict;

namespace Election {

	struct ElectionResult {
		std::map<Politician*, unsigned int> votes;
	};

	ElectionResult National(Ballot* pBallot, ElectoralDistrict* pDistrict, Country* pCountry);

}

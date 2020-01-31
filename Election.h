#pragma once
#include <vector>
#include <map>

class City;
class Ballot;
class Country;
class Politician;
class PoliticalParty;
class ElectoralDistrict;

typedef unsigned long long PopSize;

namespace Election {

	std::map<Politician*, PopSize> National(Ballot* pBallot, ElectoralDistrict* pDistrict, Country* pCountry);

}

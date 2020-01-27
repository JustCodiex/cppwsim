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

	std::map<Politician*, unsigned int> National(Ballot* pBallot, ElectoralDistrict* pDistrict, Country* pCountry);

}

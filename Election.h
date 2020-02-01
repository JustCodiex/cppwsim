#pragma once
#include "stdlib.h"

class City;
class Ballot;
class Country;
class Politician;
class PoliticalParty;
class ElectoralDistrict;

namespace Election {

	std::map<Politician*, PopSize> National(Ballot* pBallot, ElectoralDistrict* pDistrict, Country* pCountry);

}

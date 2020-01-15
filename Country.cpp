#include "Country.h"
#include "World.h"

Country::Country(std::string name) {

	m_countryName = name;

}

void Country::GenerateCountry(Random random) {

	std::vector<Weight< GovernmentType>> govTypeChances = {
		Weight(30.0f, GovernmentType::ConstitutionalMonarchism),
		Weight(50.0f, GovernmentType::AbsoluteMonarchism),
		Weight(10.0f, GovernmentType::Democracy),
	};

	m_countryGovernment.SetGovernmentType(random.Select(govTypeChances));

}

void Country::UpdateCountry(World* pWorld) {

	this->UpdateEconomy(pWorld);

}

void Country::UpdateEconomy(World* pWorld) {



}

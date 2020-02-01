#pragma once
#include "stdlib.h"
#include "Market.h"

class CountryEconomy {

public:

	CountryEconomy();

	void UpdateEconomy();

	bool HasMarket(Product productMarketType);
	bool HasMarket(Resource resourceMarketType);

private:

	std::vector<Market*> m_markets;

};

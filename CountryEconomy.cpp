#include "CountryEconomy.h"

CountryEconomy::CountryEconomy() {



}

void CountryEconomy::UpdateEconomy() {

	for (auto market : m_markets) {

		// Update the market
		market->UpdateMarket();

	}

}

bool CountryEconomy::HasMarket(Product productMarketType) {
	return false;
}

bool CountryEconomy::HasMarket(Resource resourceMarketType) {
	return false;
}

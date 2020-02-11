#include "Market.h"
#include "Company.h"
#include "EconomyLevel.h"
#include "City.h"

Market::Market() {

	m_nextMarket = 0;
	m_economyTarget = 0;

}

Market::Market(EconomyLevel* pEconomyTarget) {

	m_nextMarket = 0;
	m_economyTarget = pEconomyTarget;

}

void Market::SetNextMarket(Market* pMarket) {

	// Set next market
	m_nextMarket = pMarket;

	// Update the upper markets connection to this
	m_nextMarket->AddLowerMarket(this);

}

void Market::AddLowerMarket(Market* pMarket) {

	// Add market to previous market list
	m_prevMarkets.push_back(pMarket);

}

void Market::AddSupplyBatch(MarketProduct product, UInt32 amount, double price, Company* pSource) {

	// If new entry
	if (m_supplies[product].find(pSource) == m_supplies[product].end()) {

		// Supply to add
		MarketSupplyBatch supply;
		supply.amount = amount;
		supply.price = price;

		m_supplies[product][pSource] = supply;

	} else {

		MarketSupplyBatch& supply = m_supplies[product][pSource];

		// Add batch to our supply
		supply.amount += amount;
		supply.price = price;

	}

}

bool Market::BuySupply(MarketProduct product, UInt32 amount, UInt32& endamount, double funds, double& endprice) {
	return BuySupply(product, amount, endamount, funds, endprice, 1);
}

bool Market::BuySupply(MarketProduct product, UInt32 amount, UInt32& endamount, double funds, double& endprice, int transportFee) {

	if (m_economyTarget->GetEconomyLevel() == 0) {

		double transportCost = (((UInt64)transportFee + 1UL) * 20.0);

		for (auto sources : m_supplies[product]) {

			if (sources.second.amount >= amount && sources.second.price <= funds + transportCost) {

				sources.first->BuyStock(amount, sources.second.price + transportCost);
				endprice = sources.second.price + transportCost;
				endamount = amount;

				sources.second.amount -= amount;

				return true;

			}

		}

	} else {

		for (Market* pMarket : m_prevMarkets) {
			if (pMarket->BuySupply(product, amount, endamount, funds, endprice, transportFee)) {
				return true;
			}
		}

		return m_nextMarket->BuySupply(product, amount, endamount, funds, endprice, transportFee + 1);

	}

	return false;

}

void Market::UpdateMarket() {



}

void Market::RemoveCompanyFromMarket(Company* pCompany) {

	// Remove supplies
	m_supplies[pCompany->GetOutputType()].erase(pCompany);

	// If city
	if (m_economyTarget->GetEconomyLevel() == 0) {

		// Cast of target to city
		City* pCity = (City*)m_economyTarget;

		// Unregister the company
		pCity->UnregisterCompany(pCompany);

	}

}

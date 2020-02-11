#include "Market.h"
#include "EconomyLevel.h"
#include "City.h"

Market::Market() {

	m_nextMarket = 0;
	m_economyTarget = 0;

}

Market::Market(EconomyLevel* pEconomyTarget) {

	m_nextMarket = 0;
	m_economyTarget = pEconomyTarget;

	for (int p = 0; p < (int)MarketProduct::MARKET_PRODUCT_COUNT; p++) {
		m_supplies[(MarketProduct)p] = ProductSupply();
		m_supplies[(MarketProduct)p].requirements = GetProductRequirements((MarketProduct)p);
	}

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

void Market::UpdateMarket() {

	if (!IsLowestMarket()) {

		bool isFirstMarket = true;

		for (Market* pPrevMarkets : m_prevMarkets) {

			// Update previous markets
			pPrevMarkets->UpdateMarket();

			for (int p = 0; p < (int)MarketProduct::MARKET_PRODUCT_COUNT; p++) {

				MarketProduct m = (MarketProduct)p;
				ProductSupply* s = pPrevMarkets->GetSupply(m);

				m_supplies[m].supply += s->supply;
				m_supplies[m].profit += s->profit;

				if (isFirstMarket) {
					
					m_supplies[m].employed = s->employed;
					m_supplies[m].price = s->price;
					m_supplies[m].suppliers = s->suppliers;
					m_supplies[m].wages = s->wages;

					isFirstMarket = false;

				} else {
					
					m_supplies[m].employed += s->employed;
					m_supplies[m].price += s->price;
					m_supplies[m].suppliers += s->suppliers;
					m_supplies[m].wages += s->wages;
					
				}

			}

			for (int p = 0; p < (int)MarketProduct::MARKET_PRODUCT_COUNT; p++) {

				MarketProduct m = (MarketProduct)p;

				m_supplies[m].wages /= (Capital)m_prevMarkets.size();
				m_supplies[m].price /= (Capital)m_prevMarkets.size();

			}

		}


	} else {

		for (int p = 0; p < (int)MarketProduct::MARKET_PRODUCT_COUNT; p++) {

			MarketProduct m = (MarketProduct)p;

			double productionPrice = GetProductProductionBaseOutput(m);
			double output = (m_supplies[m].employed * productionPrice);

			m_supplies[m].supply += (UInt32)(output / m_supplies[m].suppliers);
			
			for (MarketProduct req : m_supplies[m].requirements) {
				m_supplies[req].demand += GetProductConversionRate(m) * m_supplies[m].supply;
			}

			if (m_supplies[m].supply > 0) {

				double competitionFactor = 1.0 + (m_supplies[m].demand / m_supplies[m].supply);

				m_supplies[m].price = ((GetProductProductionCost(m) + m_supplies[m].wages) / (Capital)m_supplies[m].suppliers) * competitionFactor;
				m_supplies[m].profit = (m_supplies[m].price * m_supplies[m].supply) - (m_supplies[m].employed * m_supplies[m].wages);

			}

		}

	}

}

void Market::NewCompany(MarketProduct product, UInt16 employees) {

	m_supplies[product].suppliers++;
	m_supplies[product].employed += employees;

}

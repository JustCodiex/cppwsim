#include "Market.h"
#include "EconomyLevel.h"

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

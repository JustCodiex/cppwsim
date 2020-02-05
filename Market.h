#pragma once
#include "stdlib.h"
#include "Finance.h"

class EconomyLevel;

enum class MarketLevel {
	Local,
	National,
	Global,
};

class Market {

public:

	Market();
	Market(EconomyLevel* pEconomyTarget);

	void SetNextMarket(Market* pMarket);
	
	bool IsLowestMarket() { return m_prevMarkets.size() == 0; }
	bool IsHighestMarket() { return m_nextMarket == 0; }

private:

	void AddLowerMarket(Market* pMarket);

private:

	EconomyLevel* m_economyTarget;
	
	Market* m_nextMarket;
	std::vector<Market*> m_prevMarkets;

};

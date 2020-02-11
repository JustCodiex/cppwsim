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

	struct ProductSupply {
		UInt32 supply;
		UInt32 demand;
		UInt16 suppliers; // AKA companies
		UInt16 employed;
		Capital price;
		Capital wages;
		Capital profit;
		std::vector<MarketProduct> requirements;
		ProductSupply() {
			supply = 0;
			suppliers = 0;
			employed = 0;
			price = 1.0;
			wages = 1.0;
			profit = 0.0;
			demand = 0;
		}
	};

public:

	Market();
	Market(EconomyLevel* pEconomyTarget);

	void SetNextMarket(Market* pMarket);
	
	bool IsLowestMarket() { return m_prevMarkets.size() == 0; }
	bool IsHighestMarket() { return m_nextMarket == 0; }

	void NewCompany(MarketProduct product, UInt16 employees);

	void UpdateMarket();

	EconomyLevel* GetEconomyTarget() { return m_economyTarget; }

private:

	void AddLowerMarket(Market* pMarket);

	ProductSupply* GetSupply(MarketProduct p) { return &m_supplies[p]; }

private:

	EconomyLevel* m_economyTarget;
	
	Market* m_nextMarket;
	std::vector<Market*> m_prevMarkets;
	std::map<MarketProduct, ProductSupply> m_supplies;

};

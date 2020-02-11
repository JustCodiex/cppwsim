#pragma once
#include "stdlib.h"
#include "Finance.h"

class Company;
class EconomyLevel;

enum class MarketLevel {
	Local,
	National,
	Global,
};

class Market {

public:

	struct MarketSupplyBatch {
		UInt32 amount;
		double price;
	};

public:

	Market();
	Market(EconomyLevel* pEconomyTarget);

	void SetNextMarket(Market* pMarket);
	
	bool IsLowestMarket() { return m_prevMarkets.size() == 0; }
	bool IsHighestMarket() { return m_nextMarket == 0; }

	void UpdateMarket();

	void AddSupplyBatch(MarketProduct product, UInt32 amount, double price, Company* pSource);
	bool BuySupply(MarketProduct product, UInt32 amount, UInt32& endamount, double funds, double& endprice);

	EconomyLevel* GetEconomyTarget() { return m_economyTarget; }

	void RemoveCompanyFromMarket(Company* pCompany);

private:

	bool BuySupply(MarketProduct product, UInt32 amount, UInt32& endamount, double funds, double& endprice, int transportFee);

	void AddLowerMarket(Market* pMarket);

private:

	EconomyLevel* m_economyTarget;
	
	Market* m_nextMarket;
	std::vector<Market*> m_prevMarkets;
	std::map<MarketProduct, std::map<Company*, MarketSupplyBatch>> m_supplies;

};

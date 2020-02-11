#pragma once
#include "stdlib.h"
#include "Finance.h"

enum class CompanySector {
	Primary, // Raw material extraction
	Secondary, // Manufacturing
	Tertiary, // Services
};

enum class CompanyOwnership {
	Public,
	Private,
	Voluntary,
};

class Market;

struct ProducerModule {
	UInt32 productionCapacity;
	UInt32 productionCurrent;
	UInt32 productionIncrease;
	UInt32 productionMaxIncrease;
	ProducerModule() {
		productionCapacity = productionCurrent = productionIncrease = productionMaxIncrease = 0;
	}
};

struct InputModule {
	UInt32 inputRequirements;
	UInt32 inputStock;
	InputModule() {
		inputRequirements = inputStock = 0;
	}
	double GetEfficiency() { return (inputRequirements == inputStock) ? 1.0 : inputStock / (double)inputRequirements; }
};

struct StockModule {
	UInt32 outputStock;
	UInt32 outputUnitsSold;
	double outputUnitPrice;
	double artificialPrice;
	StockModule() {
		outputStock = 0;
		outputUnitsSold = 0;
		outputUnitPrice = 0.0;
		artificialPrice = 0.0;
	}
	double GetPrice() { return outputUnitPrice + artificialPrice; }
};

class Company {

public:

	Company();
	Company(CompanySector sector, CompanyOwnership ownership);

	void SetOutputType(MarketProduct outputType) { m_outputProductType = outputType; }
	MarketProduct GetOutputType() { return m_outputProductType; }

	void SetInputType(MarketProduct inputType) { m_inputRequirementType = inputType; }
	MarketProduct GetInputType() { return m_inputRequirementType; }

	void CalculateRevenue();

	bool IsNationalCompany() { return m_markets.size() > 1; }

	void EnterMarket(Market* pMarket) { m_markets.push_back(pMarket); }
	void LeaveMarket(Market* pMarket) { m_markets.erase(FIND_VECTOR_ELEMENT(m_markets, pMarket)); }

	void UpdateCompany(Market* pMarketCaller);

	void UpdateEconomy();

	void BuyStock(UInt32 amount, double price);

	bool InDebt() { return m_savings < 0.0; }

	void CloseCompany();

private:

	void UpdateProduction();
	void UpdateRequirements(Market* pMarket);
	void UpdateStock();

	void BusinessLogic();

private:

	CompanySector m_sector;
	CompanyOwnership m_ownershipType;

	MarketProduct m_inputRequirementType;
	MarketProduct m_outputProductType;

	Capital m_income;
	Capital m_outcome;
	Capital m_savings;

	double m_employeeWage;
	unsigned short m_employeeCount;

	ProducerModule m_productionModule;
	StockModule m_stockModule;
	InputModule m_inputModule;

	std::vector<Market*> m_markets;

};

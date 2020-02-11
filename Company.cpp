#include "Company.h"
#include "Market.h"

Company::Company() {

	m_sector = CompanySector::Primary;
	m_ownershipType = CompanyOwnership::Private;

	m_inputRequirementType = m_outputProductType = MarketProduct::Undefined;

	// Set default monetary values
	m_savings = 1000.0;
	m_income = m_outcome = 0;

	m_employeeCount = 1;
	m_employeeWage = 5.0;

}

Company::Company(CompanySector sector, CompanyOwnership ownership) {

	m_sector = sector;
	m_ownershipType = ownership;

	// Set default monetary values
	m_savings = 1000.0;
	m_income = m_outcome = 0;

	// Set default market input and output
	m_inputRequirementType = m_outputProductType = MarketProduct::Undefined;

	m_employeeCount = 1;
	m_employeeWage = 5.0;

}

void Company::CalculateRevenue() {
	m_savings += m_income - m_outcome;
}

void Company::UpdateCompany(Market* pMarketCaller) {

	if (CONTAINS_VECTOR_ELEMENT(m_markets, pMarketCaller)) {

		if (m_sector == CompanySector::Primary && IsMarketMaterial(m_outputProductType)) {

			// Update our current resource production
			this->UpdateProduction();

			// Update the stock
			this->UpdateStock();

			// Add supply batch
			pMarketCaller->AddSupplyBatch(m_outputProductType, m_stockModule.outputStock, m_stockModule.GetPrice(), this);

		} else if (m_sector == CompanySector::Secondary && IsMarketGoods(m_outputProductType)) {

			this->UpdateProduction();

			this->UpdateRequirements(pMarketCaller);

			this->UpdateStock();

			pMarketCaller->AddSupplyBatch(m_outputProductType, m_stockModule.outputStock, m_stockModule.GetPrice(), this);

		}

	}

}

void Company::UpdateProduction() {

	m_productionModule.productionCapacity = (UInt32)(m_employeeCount * GetProductProductionBaseOutput(m_outputProductType));

	if (m_productionModule.productionCurrent > m_productionModule.productionCapacity) {
		m_productionModule.productionCurrent = m_productionModule.productionCapacity;
	} else {
		m_productionModule.productionMaxIncrease = m_productionModule.productionCapacity - m_productionModule.productionCurrent;
		if (m_productionModule.productionIncrease + 1 <= m_productionModule.productionMaxIncrease) {
			m_productionModule.productionIncrease++;
		} else {
			m_productionModule.productionIncrease--;
		}
		m_productionModule.productionCurrent += m_productionModule.productionIncrease;
	}

}

void Company::UpdateStock() {

	m_stockModule.outputStock += (UInt32)(m_productionModule.productionCurrent * m_inputModule.GetEfficiency());
	m_stockModule.outputUnitPrice = (1.0 / m_stockModule.outputStock) + GetProductProductionCost(m_outputProductType);

	m_outcome += m_productionModule.productionCurrent * GetProductProductionCost(m_outputProductType);

}

void Company::UpdateRequirements(Market* pMarket) {

	// Get the input requirements
	m_inputModule.inputRequirements = m_productionModule.productionCurrent * GetProductConversionRate(m_outputProductType);

	// Remember our final price
	double endprice = 0.0;
	UInt32 endstock = 0;

	// Buy the supply we need
	if (pMarket->BuySupply(m_inputRequirementType, m_inputModule.inputRequirements, endstock, m_savings, endprice)) {

		// Add to stock
		m_inputModule.inputStock += endstock;

		// Add price to outcome
		m_outcome += endprice;

	}

}

void Company::BuyStock(UInt32 amount, double price) {

	m_stockModule.outputUnitsSold += amount;
	m_stockModule.outputStock -= amount;

	m_income += amount * price;

}

void Company::UpdateEconomy() {

	// Add employee wages to outcome
	m_outcome += m_employeeCount * m_employeeWage;

	// Calculate revenue
	this->CalculateRevenue();

	// Update business logic
	this->BusinessLogic();

	// Reset monthly values
	m_outcome = 0;
	m_income = 0;
	m_stockModule.outputUnitPrice = 0;

}

void Company::BusinessLogic() {

	if (m_outcome > m_income) {

		if (m_employeeCount > 1) {

			if (m_income >= m_outcome - m_employeeWage) {
				m_employeeCount--;
			} else {
				m_employeeWage -= 0.25;
				m_stockModule.artificialPrice += 0.2;
			}

		} else {

			m_stockModule.artificialPrice += 0.15;

		}

	} else {

		if (m_income > (m_outcome + m_employeeWage) * 1.2) {
			m_employeeCount++;
		} else {
			m_employeeWage += 0.025;
		}

	}

}

void Company::CloseCompany() {

	// For all markets we partake in
	for (auto market : m_markets) {

		// Leave the market
		this->LeaveMarket(market);

		// Remove ourselves from the market
		market->RemoveCompanyFromMarket(this);

	}

}

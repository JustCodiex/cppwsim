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

class Company {

public:

	Company();
	Company(CompanySector sector, CompanyOwnership ownership);

	void SetParentCompany(Company* pParent);

	void SetOutputType(MarketProduct outputType) { m_outputProductType = outputType; }
	MarketProduct GetOutputType() { return m_outputProductType; }

	void SetInputType(MarketProduct inputType) { m_inputRequirementType = inputType; }
	MarketProduct GetInputType() { return m_inputRequirementType; }

	void CalculateRevenue();

private:

	CompanySector m_sector;
	CompanyOwnership m_ownershipType;

	Company* m_parentCompany;
	std::vector<Company*> m_subsidiaries;

	MarketProduct m_inputRequirementType;
	MarketProduct m_outputProductType;

	Capital m_income;
	Capital m_outcome;
	Capital m_savings;

};

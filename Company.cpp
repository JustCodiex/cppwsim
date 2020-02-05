#include "Company.h"

Company::Company() {

	m_parentCompany = 0;

	m_sector = CompanySector::Primary;
	m_ownershipType = CompanyOwnership::Private;

	// Set default monetary values
	m_savings = m_income = m_outcome = 0;

}

Company::Company(CompanySector sector, CompanyOwnership ownership) {

	m_parentCompany = 0;

	m_sector = sector;
	m_ownershipType = ownership;

	// Set default monetary values
	m_savings = m_income = m_outcome = 0;

}

void Company::SetParentCompany(Company* pParent) {
	
	// Do we already have a parent company?
	if (m_parentCompany != NULL) {

		// Remove ourselves as subsidiary
		m_parentCompany->m_subsidiaries.erase(std::find(m_parentCompany->m_subsidiaries.begin(), m_parentCompany->m_subsidiaries.end(), this));

	}

	// Make sure parent is NULL
	if (pParent != NULL) {

		// Set parent company
		m_parentCompany = pParent;

		// Add ourselves as a subsidiary to the parent company
		m_parentCompany->m_subsidiaries.push_back(this);

	}

}

void Company::CalculateRevenue() {
	m_savings += m_income - m_outcome;
}

#include "Market.h"

Market::Market() {
	
	m_resource = Resource::None;
	m_product = Product::None;
	m_isResourceMarket = false;

}

void Market::AddSupplier(Company* pSupplyCompany) {
	m_suppliers.push_back(pSupplyCompany);
}

void Market::AddDemander(Company* pDemandCompany) {
	m_demanders.push_back(pDemandCompany);
}

void Market::SetResourceMarket(Resource resource) {
	m_isResourceMarket = true;
	m_resource = resource;
	m_product = Product::None;
}

void Market::SetProductMarket(Product product) {
	m_isResourceMarket = false;
	m_product = product;
	m_resource = Resource::None;
}

void Market::UpdateMarket() {



}

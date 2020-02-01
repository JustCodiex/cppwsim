#pragma once
#include "stdlib.h"
#include "Company.h"

enum class Resource {

	None,
	Timber,
	Coal,
	Oil,
	Steel,
	Grain,
	Meat,
	Produce, // Generic term for veggies, fruits ++
	Wool,

};

enum class Product {

	None,
	Furniture,
	Fabric, // + Clothes
	Heat,
	Weapons,
	Transport, // Cars, Trains, Boats, Aircrafts eg.
	Food,
	Electricity,
	Tools,
	Machinery,

};

class Market {

public:

	Market();

	void AddSupplier(Company* pSupplyCompany);
	void AddDemander(Company* pDemandCompany);

	void SetResourceMarket(Resource resource);
	void SetProductMarket(Product product);

	bool IsResourceMarket() { return m_isResourceMarket; }
	bool IsProductMarket() { return !m_isResourceMarket; }

	void UpdateMarket();

private:

	Resource m_resource;
	Product m_product;

	bool m_isResourceMarket;

	std::vector<Company*> m_suppliers;
	std::vector<Company*> m_demanders;

};

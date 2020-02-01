#pragma once
#include "stdlib.h"

class City;

enum class CompanyType {
	Unknown,
	Extraction,
	Manufacturing, // Production
	Logistics,
	Sales,
	Construction,
	Service,
};

class Company {

public:

	struct Branch {

		City* location;

		unsigned int employed;

		double income;
		double outcome; // AKA expenditure


	};

public:

	virtual CompanyType GetType() = 0;

	//bool IsMakingAProfit() { return m_income > m_outcome; }



protected:

	std::vector< Branch> m_branches;

private:



};

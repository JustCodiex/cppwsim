#pragma once
#include "Company.h"

class SalesCompany : public Company {

public:

private:

	CompanyType GetType() override { return CompanyType::Sales; }

};

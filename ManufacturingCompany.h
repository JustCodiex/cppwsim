#pragma once
#include "Company.h"

class ManufacturingCompany : public Company {

public:

private:

	CompanyType GetType() override { return CompanyType::Manufacturing; }

};

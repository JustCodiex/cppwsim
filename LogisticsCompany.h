#pragma once
#include "Company.h"

class LogisticsCompany : public Company {

public:

private:

	CompanyType GetType() override { return CompanyType::Logistics; }

};

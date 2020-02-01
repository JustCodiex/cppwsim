#pragma once
#include "Company.h"

class ServiceCompany : public Company {

public:

private:

	CompanyType GetType() override { return CompanyType::Service; }

};

#pragma once
#include "Company.h"

class ConstructionCompany : public Company {

public:

private:

	CompanyType GetType() override { return CompanyType::Construction; }

};

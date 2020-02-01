#pragma once
#include "Company.h"

class ExtractionCompany : public Company {

public:

private:

	CompanyType GetType() override { return CompanyType::Extraction; }

};

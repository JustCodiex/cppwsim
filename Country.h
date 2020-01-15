#pragma once
#include <string>

class Country {

public:

	Country(std::string name);

	std::string GetName() { return m_countryName; }

private:

	std::string m_countryName;

};

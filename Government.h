#pragma once
#include <string>

enum class GovernmentType {
	Democracy, // Rule of many (Republic)
	Oligarchy, // Rule of few (Aristocracy, think wealthy elite)
	ConstitutionalMonarchism,
	AbsoluteMonarchism,
	Anarchism, // Rule of none
};

std::string const getGovTypeName(GovernmentType type);

class Government {

public:

	Government();

	void SetGovernmentType(GovernmentType type) { m_govType = type; }
	GovernmentType GetGovernmentType() { return m_govType; }

private:

	GovernmentType m_govType;

};

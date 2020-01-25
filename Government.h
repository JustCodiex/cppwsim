#pragma once
#include <map>
#include <string>
#include "GovernmentMinistry.h"
#include "LegislativeCoalition.h"

class Country;
class LegislativeChamber;

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

	// Form a new government from a legislative chamber
	void NewGovernment(LegislativeChamber* chamber);

	// Appoint a new government
	void AppointGovernment();

	// Elect a new government
	void ElectGovernment();

	Country* GetCountry() { return m_targetCountry; }
	void SetCountry(Country* pCountry) { m_targetCountry = pCountry; }

	Politician* GetHeadOfGovernment() { return m_govMinistries[PolicyArea::State].GetMinister(); }

	size_t GetMinistryCount() { return m_govMinistries.size(); }

	LegislativeCoalition* GetCoalition() { return &m_govCoalition; }

private:

	Country* m_targetCountry;
	GovernmentType m_govType;

	LegislativeCoalition m_govCoalition;
	std::map<PolicyArea, GovernmentMinistry> m_govMinistries;

};

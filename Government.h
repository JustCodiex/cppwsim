#pragma once
#include <map>
#include <string>
#include "TimeDate.h"
#include "GovernmentMinistry.h"
#include "LegislativeCoalition.h"

class Country;
class Royal;
class LegislativeChamber;

enum class GovernmentType {
	Democracy, // Rule of many (Republic)
	Oligarchy, // Rule of few (Aristocracy, think wealthy elite)
	ConstitutionalMonarchism,
	AbsoluteMonarchism,
	Anarchism, // Rule of none
};

enum class GovernmentElectoralSystem {
	WinnerTakesAll,
	TwoRoundSystem,
	ElectoralCollege, // If StateCount > 1 then use states, otherwise use regions
};

std::string const getGovTypeName(GovernmentType type);

class Government {

public:

	Government();

	void SetGovernmentType(GovernmentType type) { m_govType = type; }
	GovernmentType GetGovernmentType() { return m_govType; }

	// Form a new government from a legislative chamber
	void NewGovernment(LegislativeChamber* chamber, TimeDate date);

	// Appoint a new government
	void AppointGovernment(Random random, TimeDate date);

	// Elect a new government
	void ElectGovernment(TimeDate date);

	Country* GetCountry() { return m_targetCountry; }
	void SetCountry(Country* pCountry) { m_targetCountry = pCountry; }

	Politician* GetHeadOfGovernment() { return m_govMinistries[PolicyArea::State].GetMinister(); }

	size_t GetMinistryCount() { return m_govMinistries.size(); }

	void SetTermLength(char lim) { m_govTerm = lim; }
	int GetTermLength() { return m_govTerm; }

	TimeDate GetNextFormDate() { return m_govFormDate; }

	LegislativeCoalition* GetCoalition() { return &m_govCoalition; }

	GovernmentElectoralSystem GetElectionSystem() { return m_electionSystem; }
	void SetElectionSystem(GovernmentElectoralSystem sys) { m_electionSystem = sys; }

private:

	void GetGovernmentRating();

	void AppointNewMinister(Random random, PolicyArea area, Royal* royal, bool considerParliament);
	void RemoveMinistry(GovernmentMinistry* ministry);

private:

	Country* m_targetCountry;
	GovernmentType m_govType;

	float m_govSupport;

	char m_govTerm;
	TimeDate m_govFormDate;
	TimeDate m_govNextFormDate;

	LegislativeCoalition m_govCoalition;
	std::map<PolicyArea, GovernmentMinistry> m_govMinistries;

	GovernmentElectoralSystem m_electionSystem;

};

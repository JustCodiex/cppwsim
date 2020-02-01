#pragma once
#include "stdlib.h"
#include "PolicyArea.h"
#include "Politician.h"

class GovernmentMinistry {

public:

	GovernmentMinistry();

	void SetMinister(Politician* pMinister) { m_minister = pMinister; }
	Politician* GetMinister() { return m_minister; }

	// What's the public opinion of the running of this ministry?
	float GetMinistryApproval() { return m_apporval; }

	static std::string GetMinistryName(PolicyArea are);

private:

	float m_apporval;
	Politician* m_minister;

};

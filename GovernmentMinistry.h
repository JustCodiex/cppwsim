#pragma once
#include <string>
#include <vector>
#include "PolicyArea.h"
#include "Politician.h"

class GovernmentMinistry {

public:

	GovernmentMinistry();

	void SetMinister(Politician* pMinister) { m_minister = pMinister; }
	Politician* GetMinister() { return m_minister; }

	static std::string GetMinistryName(PolicyArea are);

private:

	Politician* m_minister;

};

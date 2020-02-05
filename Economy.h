#pragma once
#include "Finance.h"
#include "EconomyLevel.h"

class Economy {

public:

	Economy();
	Economy(EconomyLevel* economyTarget);

	void UpdateEconomy();

private:

	EconomyLevel* m_target;

};

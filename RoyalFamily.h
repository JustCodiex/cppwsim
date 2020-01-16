#pragma once
#include "Person.h"
#include <vector>

class RoyalFamily {

public:

	struct RoyalCouple {
		Person* l;
		Person* r;
		std::vector<RoyalCouple> children;
		RoyalCouple() {
			l = r = 0;
		}
	};

public:

	RoyalFamily();

	void CreateFamily(Random random);

	Person* GetHeadOfFamily() { return (m_rootCouple.l != 0) ? m_rootCouple.l : m_rootCouple.r; }

private:

	RoyalCouple m_rootCouple;

};

#pragma once
#include "Royal.h"
#include <vector>

class RoyalFamily {

public:

	struct RoyalCouple {
		Royal* l;
		Royal* r;
		std::vector<RoyalCouple> children;
		RoyalCouple() {
			l = r = 0;
		}
	};

public:

	RoyalFamily();

	void CreateFamily(Random random);

	Royal* GetHeadOfFamily() { return (m_rootCouple.l != 0) ? m_rootCouple.l : m_rootCouple.r; }

	void UpdateRoyalFamily();

private:

	RoyalCouple m_rootCouple;

};

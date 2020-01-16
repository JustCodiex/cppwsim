#include "RoyalFamily.h"

RoyalFamily::RoyalFamily() {

	m_rootCouple.l = 0;
	m_rootCouple.r = 0;

}

void RoyalFamily::CreateFamily(Random random) {

	m_rootCouple.l = new Person(random.NextInt(26, 60), Sex::Male, random);
	m_rootCouple.r = new Person(random.NextInt(26, 60), Sex::Female, random);

	int children = random.NextInt(1, 13);
	int minChildAge = (m_rootCouple.r->GetAge() - 30 >= 0) ? m_rootCouple.r->GetAge() - 30 : 0;

	for (int i = 0; i < children; i++) {

		RoyalCouple couple;
		couple.l = new Person(random.NextInt(0, m_rootCouple.r->GetAge() - 18), random);

		m_rootCouple.children.push_back(couple);

	}

}

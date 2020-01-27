#include "RoyalFamily.h"

RoyalFamily::RoyalFamily() {

	m_rootCouple.l = 0;
	m_rootCouple.r = 0;

}

void RoyalFamily::CreateFamily(Random random) {

	m_rootCouple.l = new Royal(random.NextInt(26, 60), Sex::Male, random);
	m_rootCouple.l->SetRoyalTitle(RoyalTitle::King);

	m_rootCouple.r = new Royal(random.NextInt(26, 60), Sex::Female, random);
	m_rootCouple.r->SetRoyalTitle(RoyalTitle::Queen);

	int children = random.NextInt(1, 13);
	int minChildAge = (m_rootCouple.r->GetAge() - 30 >= 0) ? m_rootCouple.r->GetAge() - 30 : 0;

	for (int i = 0; i < children; i++) {

		RoyalCouple couple;
		couple.l = new Royal(random.NextInt(0, m_rootCouple.r->GetAge() - 18), random);

		if (couple.l->GetSex() == Sex::Male) {
			couple.l->SetRoyalTitle((i == 0) ? RoyalTitle::CrownPrince : RoyalTitle::Prince);
		} else {
			couple.l->SetRoyalTitle((i == 0) ? RoyalTitle::CrownPrincess : RoyalTitle::Princess);
		}

		m_rootCouple.children.push_back(couple);

	}

}

void RoyalFamily::UpdateRoyalFamily() {



}

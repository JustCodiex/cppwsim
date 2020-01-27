#pragma once
#include "Person.h"

enum class RoyalTitle {
	King,
	Queen,
	CrownPrince,
	CrownPrincess,
	Prince,
	Princess,
};

class Royal : public Person {

public:

	Royal();
	Royal(Random random);
	Royal(unsigned char age, Random random);
	Royal(Sex sex, Random random);
	Royal(unsigned char age, Sex sex, Random random);

	float GetPopularity() { return m_popularity; }

	std::string GetTitle() override;

	void SetRoyalTitle(RoyalTitle title) { m_royalTitle = title; }
	RoyalTitle GetRoyalTitle() { return m_royalTitle; }

private:

	float m_popularity;
	RoyalTitle m_royalTitle;

};

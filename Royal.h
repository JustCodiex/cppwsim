#pragma once
#include "Person.h"

class Royal : public Person {

public:

	Royal();
	Royal(Random random);
	Royal(unsigned char age, Random random);
	Royal(Sex sex, Random random);
	Royal(unsigned char age, Sex sex, Random random);

	float GetPopularity() { return m_popularity; }

private:

	float m_popularity;

};

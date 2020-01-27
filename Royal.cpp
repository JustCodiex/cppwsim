#include "Royal.h"

Royal::Royal() : Person() {
	m_popularity = 1.0f;
	m_royalTitle = RoyalTitle::Prince;
}

Royal::Royal(Random random) : Person(random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
	m_royalTitle = RoyalTitle::Prince;
}

Royal::Royal(unsigned char age, Random random) : Person(age, random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
	m_royalTitle = RoyalTitle::Prince;
}

Royal::Royal(Sex sex, Random random) : Person(sex, random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
	m_royalTitle = RoyalTitle::Prince;
}

Royal::Royal(unsigned char age, Sex sex, Random random) : Person(age, sex, random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
	m_royalTitle = RoyalTitle::Prince;
}

std::string Royal::GetTitle() {
	std::string str;
	switch (m_royalTitle) {
	case RoyalTitle::King:
		str = "King";
		break;
	case RoyalTitle::Queen:
		str = "Queen";
		break;
	case RoyalTitle::CrownPrince:
		str = "Crow Prince";
		break;
	case RoyalTitle::CrownPrincess:
		str = "Crow Princess";
		break;
	case RoyalTitle::Prince:
		str = "Prince";
		break;
	case RoyalTitle::Princess:
		str = "Princess";
		break;
	default:
		break;
	}
	return str + " " + this->GetFullName();
}

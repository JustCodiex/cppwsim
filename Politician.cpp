#include "Politician.h"
#include "LegislativeSeat.h"

Politician::Politician():Person() {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = PolicyArea::State;
}

Politician::Politician(Random random):Person(random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
}

Politician::Politician(Sex sex, Random random) : Person(sex, random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
}

Politician::Politician(unsigned char age, Random random) : Person(age, random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
}

Politician::Politician(unsigned char age, Sex sex, Random random) : Person(age, sex, random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
}

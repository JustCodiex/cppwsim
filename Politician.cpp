#include "Politician.h"
#include "LegislativeSeat.h"

Politician::Politician():Person() {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = PolicyArea::State;
	m_title = PoliticalTitle::None;
}

Politician::Politician(Random random):Person(random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
	m_title = PoliticalTitle::None;
}

Politician::Politician(Sex sex, Random random) : Person(sex, random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
	m_title = PoliticalTitle::None;
}

Politician::Politician(unsigned char age, Random random) : Person(age, random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
	m_title = PoliticalTitle::None;
}

Politician::Politician(unsigned char age, Sex sex, Random random) : Person(age, sex, random) {
	m_loyalty = 0.0f;
	m_party = 0;
	m_seat = 0;
	m_speciality = (PolicyArea)random.NextInt(0, (int)PolicyArea::_COUNT_);
	m_title = PoliticalTitle::None;
}

std::string Politician::GetTitle() {
	std::string str;
	switch (m_title) {
	case PoliticalTitle::MP:
		str = "MP ";
		break;
	case PoliticalTitle::Secretary:
		str = "Secretary ";
		break;
	case PoliticalTitle::President:
		str = "President ";
		break;
	case PoliticalTitle::PrimeMinister:
		str = "Prime Minister ";
		break;
	case PoliticalTitle::Minister:
		str = "Minister ";
		break;
	case PoliticalTitle::Lord:
		str = "Lord ";
		break;
	case PoliticalTitle::None:
	default:
		str = "";
		break;
	}
	return str + this->GetFullName();
}

void Politician::UpdateTitle(PoliticalTitle title) {
	
	// Update if new title is better than current OR we lose or MP status (because that doesn't stick around)
	if (title >= m_title || (title < PoliticalTitle::MP && m_title <= PoliticalTitle::MP)) {
		m_title = title;
	}

}

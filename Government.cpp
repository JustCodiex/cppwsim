#include "Government.h"

std::string const getGovTypeName(GovernmentType type) {
	switch (type) {
    case GovernmentType::Democracy:
        return "Democracy";
    case GovernmentType::Oligarchy:
        return "Oligarchy";
    case GovernmentType::ConstitutionalMonarchism:
        return "Constitutional Monarchy";
    case GovernmentType::AbsoluteMonarchism:
        return "Absolute Monarchy";
    case GovernmentType::Anarchism:
        return "Anarchism";
    default:
        return "Unknown";
	}
}

Government::Government() {

	m_govType = GovernmentType::Anarchism;

}

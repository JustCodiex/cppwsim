#include "Ballot.h"

Ballot::Ballot() {

}

void Ballot::AddCandidate(Politician* pPolitician) {
	m_candidates.push_back(pPolitician);
}

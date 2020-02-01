#pragma once
#include "stdlib.h"

class Politician;

class Ballot {

public:

	Ballot();

	void AddCandidate(Politician* pPolitician);

	int GetBallotSize() { return (int)m_candidates.size(); }

	std::vector<Politician*> GetCandidates() { return m_candidates; }

private:

	std::vector<Politician*> m_candidates;

};

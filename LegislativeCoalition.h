#pragma once
#include "stdlib.h"
#include "PoliticalParty.h"

enum class CoalitionType {
	None,
	Majority, 
	Minority,
	Opposition,
};

struct LegislativeCoalition {
	int seats;
	int seatMajority;
	int seatsSupplied;
	int conflictingPartners;
	float stability;
	PoliticalParty* leader;
	CoalitionType type;
	std::string name;
	std::vector<PoliticalParty*> partners;
	LegislativeCoalition() {
		name = "";
		seats = seatMajority = seatsSupplied = 0;
		leader = 0;
		stability = 0.0f;
		conflictingPartners = 0;
		type = CoalitionType::None;
	}
	LegislativeCoalition(LegislativeCoalition* coalition) {
		name = coalition->name;
		seats = coalition->seats;
		seatsSupplied = coalition->seatsSupplied;
		seatMajority = coalition->seatMajority;
		leader = coalition->leader;
		type = coalition->type;
		partners = coalition->partners;
		stability = coalition->stability;
		conflictingPartners = coalition->conflictingPartners;
	}
};

#define IsStableCoalition(col) col.stability == 1.0f
#define IsVaryUnstableCoalition(col) col.stability <= 0.5f
#define IsUnstableCoalition(col) col.stability <= 1.0f && col.stability > 0.5f

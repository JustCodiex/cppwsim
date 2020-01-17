#pragma once
#include "Random.h"
#include <string>
#include "Ideology.h"

const std::string conservative_party_names[] = { 
	"", "", "" 
};

const std::string reactionary_party_names[] = { 
	GetIdeologyName(POLITICAL_IDEOLOGY::ID_REACTIONARY), 
	"Party of Reactionaries", "The Reactionary Party", "Loyalists", "Party of Loyalists", "Reactionary Party", "Loyalist Party",
	"Reactionary Front", "Loyal Front", "Loyalist Front", "New Reactionaries", "Reactionary Alliance", "Alliance of Reactionaries",
	"Royalists", "Reactionaries",
};

const std::string reformist_party_names[] = {
	GetIdeologyName(POLITICAL_IDEOLOGY::ID_REFORMIST),
	"National Reformers", "Reformists", "Reformers", "Reform Party", "Party of Reforms", "Renewal Party", "Alliance for Reform",
	"Alliance of Reforms", "Alliance for Reforms",
};

const std::string radicalist_party_names[] = {
	GetIdeologyName(POLITICAL_IDEOLOGY::ID_RADICALISM),
	"Radicalist Party", "Preservation Party", "Radicals", "Radicalists", "Radical Party", "Radical Alliance", "Radical Front",
};

const std::string GetPartyName(POLITICAL_IDEOLOGY ideology, Random random) {
	
	switch (ideology) {
	case POLITICAL_IDEOLOGY::ID_NATIONAL_CONSERVATISM:
	case POLITICAL_IDEOLOGY::ID_SOCIAL_CONSERVATISM:
		return conservative_party_names[random.NextInt(0, lengthof(conservative_party_names, std::string))];
	case POLITICAL_IDEOLOGY::ID_REACTIONARY:
		return reactionary_party_names[random.NextInt(0, lengthof(reactionary_party_names, std::string))];
	case POLITICAL_IDEOLOGY::ID_REFORMIST:
		return reformist_party_names[random.NextInt(0, lengthof(reformist_party_names, std::string))];
	case POLITICAL_IDEOLOGY::ID_RADICALISM:
		return radicalist_party_names[random.NextInt(0, lengthof(radicalist_party_names, std::string))];
	default:
		return GetIdeologyName(ideology);
	}

}

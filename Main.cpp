/////////////////////////////////////
// Standard Includes
/////////////////////////////////////
#include <stdio.h>
#include <iostream>

/////////////////////////////////////
// Simulation Includes
/////////////////////////////////////
#include "World.h"
#include "Country.h"
#include "Random.h"
#include "Person.h"
#include "TimeDate.h"
#include "StringUtilities.h"

void printLegChamber(LegislativeChamber* pChamber) {
	std::cout << "\t\t" << pChamber->GetName() << ": " << pChamber->GetSeatCount() << " seats, " << (int)pChamber->GetTermLimit() << " year term. " << std::endl;
	if (pChamber->IsElectableChamber()) {
		std::cout << "\t\t\tLast election: " << pChamber->GetLastElectionDate().toString() << std::endl;
		std::cout << "\t\t\tNext election: " << pChamber->GetNextElectionDate().toString() << std::endl;
	}
}

int main() {

	std::cout << "World Simulator (C++)" << std::endl;

	Random random;

	World* pWorld = new World("Simulation World");
	pWorld->Generate(random);

	std::vector<Country*> worldCountries = pWorld->GetCountries();
	std::cout << "Generated the world: " << pWorld->GetName() << " with " << worldCountries.size() << " countries" << std::endl;
	
	for (size_t i = 0; i < worldCountries.size(); i++) {
		Country* country = worldCountries[i];
		std::cout << "[" << (i + 1) << "] " << country->GetName() << " (Head of State: " << country->GetHeadOfState()->GetFullName() << ")" << std::endl;
		std::cout << "\tPopulation: " << to_quantity(country->GetPopulationSize()) << std::endl;
		std::cout << "\tGovernment: " << getGovTypeName(country->GetGovernment()->GetGovernmentType()) << std::endl;
		Legislature* legislature = country->GetLegislature();
		if (legislature) {
			std::cout << "\tLegislature: " << std::endl;
			if (legislature->IsBicameral()) {
				printLegChamber(legislature->GetChamber(true));
				printLegChamber(legislature->GetChamber(false));
			} else {
				printLegChamber(legislature->GetChamber(true));
			}
		}
		std::cout << "\tParties: " << std::endl;
		std::vector<PoliticalParty> parties = country->GetPartyList();
		for (int i = 0; i < parties.size(); i++) {
			std::cout << "\t\t" << parties[i].GetName() << " (" << parties[i].GetShort() << "), " << GetIdeologyName(parties[i].GetIdeology()->GetIdeology());
			std::cout << ", established in " << parties[i].GetYear() << std::endl;
		}
	}

	system("pause");
	system("cls");

	TimeDate date = TimeDate(1800, 1, 1);

	while (!date.isDate(2100, 1, 1)) {

		std::cout << "World Simulator (C++)" << std::endl;
		std::cout << "Date: " << date.toString() << std::endl;

		pWorld->UpdateWorld(date);

		// Print additional data here

		system("cls");
		date = date.nextDay();

	}

	system("pause");

	// Cleanup :D
	delete pWorld;

}

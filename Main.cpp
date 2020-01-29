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

TimeDate TimeDate::Decade = TimeDate(10, 1, 1);
TimeDate TimeDate::Century = TimeDate(100, 1, 1);

void printLegChamber(LegislativeChamber* pChamber) {
	std::cout << "\t\t" << pChamber->GetName() << ": " << pChamber->GetSeatCount() << " seats, " << (int)pChamber->GetTermLength() << " year term. " << std::endl;
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
		std::cout << "[" << (i + 1) << "] " << country->GetName() << " (Head of State: " << country->GetHeadOfState()->GetTitle() << ")" << std::endl;
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
		std::cout << "\tNational Parties: " << std::endl;
		std::vector<Country::Party> parties = country->GetPartyList();
		for (int i = 0; i < parties.size(); i++) {
			std::cout << "\t\t" << parties[i].party->GetName() << " (" << parties[i].party->GetShort() << "), " << GetIdeologyName(parties[i].party->GetIdeology()->GetIdeology());
			std::cout << ", established in " << parties[i].party->GetYear() << std::endl;
		}
	}

	system("pause");
	system("cls");

	TimeDate date = TimeDate(1800, 1, 1);

	bool realtime = true; // Disable this and the simulation goes A LOT faster

	while (!date.isDate(2100, 1, 1)) {

		if (realtime) {

			std::cout << "World Simulator (C++)" << std::endl;
			std::cout << "Date: " << date.toString() << std::endl;
		}

		pWorld->UpdateWorld(date);

		// Print additional data here

		if (realtime) {
			system("cls");
		}
		
		date = date.nextDay();

	}

	system("pause");

	// Cleanup :D
	delete pWorld;

}

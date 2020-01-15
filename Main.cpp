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
#include "TimeDate.h"

int main() {

	std::cout << "World Simulator (C++)" << std::endl;

	Random random;

	World* pWorld = new World("Simulation World");
	pWorld->Generate(random);

	std::vector<Country*> worldCountries = pWorld->GetCountries();
	std::cout << "Generated the world: " << pWorld->GetName() << " with " << worldCountries.size() << " countries" << std::endl;
	
	for (size_t i = 0; i < worldCountries.size(); i++) {
		std::cout << "[" << (i + 1) << "] " << worldCountries.at(i)->GetName() << std::endl;
	}

	system("pause");
	system("cls");

	TimeDate date = TimeDate(1800, 1, 1);

	while (!date.isDate(2100, 1, 1)) {

		std::cout << "World Simulator (C++)" << std::endl;
		std::cout << "Date: " << date.getNameOfMonth() << " " << date.day << ", " << date.year << std::endl;

		date = date.nextDay();

		system("cls");

	}

	system("pause");

	// Cleanup :D
	delete pWorld;

}

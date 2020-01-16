#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "Random.h"

const std::vector<std::string> maleNames = {
	"Henry", "Harry", "James", "John", "Jonas", "Jonathan", "Oswald", "Larry", "Percival", "Rick", "Morty", "Garry", "Allan", "Frederick", "Alex",
	"Alexander", "Thomas", "Joe", "Cameron", "David",
};

const std::vector<std::string> femaleNames = {
	"Alexandria", "Sophia", "Tara", "Freya", "Victoria", "Monica", "Jillian", "Jill", "Elizabeth", "Margaret", "Aubrey", "Amy", "Emilty", "Emilia", "Jo",
	"Karla", "Kaya", "Kate",
};

const std::vector<std::string> middleNames = {
	"Ostergaard", "Vestergaard", "Lokke", "Lunde", "", "Scavenius",
};

const std::vector<std::string> lastNames = {
	"Plaza", "Pratt", "Black", "White", "Green", "Brown", "Johnson", "Doe", "Potter", "Frederiksen", "Rasmussen", "Stein", "Steiner", "Larsson", "Shulz", "Preker", "Reichswald",
	"Stauning", "Dove", "Sturgeon", "Middleton", "Middleditch",
};

const std::string RandomMaleName(Random random) { return maleNames[random.NextInt(0, (int)maleNames.size())]; }

const std::string RandomFemaleName(Random random) { return femaleNames[random.NextInt(0, (int)femaleNames.size())]; }

const std::string RandomMiddleName(Random random) { return middleNames[random.NextInt(0, (int)middleNames.size())]; }

const std::string RandomLastName(Random random) { return lastNames[random.NextInt(0, (int)lastNames.size())]; }

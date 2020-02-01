#pragma once
#include "stdlib.h"

std::vector<std::string> cityNames = {
	"Pawnee", "Eagleton", "Horslet", "Hornslet", "Bakersfield", "Whitehall", "Green Valley", "Yorktown",
	"Hampshire", "Rockshire", "Rocksville", "Hornets Valley", "Cloverfield", "Rockfield", "Sleepy Hollows",
	"Springfield", "Yokyo", "Freeville", "Johnson's Creek", "Redwoods", "Holy Springs", "Hacksville", "Brooksville",
	"Brooklyn", "Gold Valley", "", "", "", "", "", "", "", "", 
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

std::string nextCityName(Random random) {
	return cityNames[random.NextInt(0, (int)cityNames.size())];
}

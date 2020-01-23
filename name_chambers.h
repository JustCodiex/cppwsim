#pragma once
#include <string>
#include "Random.h"

std::string lower_house_names[] = {
	"Lower House", "House of Commons", "House of Deputies", "House of Representatives", "House of Delegates", "Chamber of Delegates", 
	"Chamber of Representatives", "Chamber of Deputies", "Chamber of Commons", "Lower Chamber", "Second Chamber", "Secondary Chamber",
	"House of Peoples", "Chamber of Peoples",
};

std::string upper_house_names[] = {
	"Upper House", "House of Lords", "House of Seniors", "House of Senators", "Senate", "Upper Chamber", "First Chamber",
	"Primary Chamber",
};

std::string GetChamberName(Random random, bool isFirstChamber) {
	if (isFirstChamber) {
		return lower_house_names[random.NextInt(0, sizeof(lower_house_names) / sizeof(std::string))];
	} else {
		return upper_house_names[random.NextInt(0, sizeof(upper_house_names) / sizeof(std::string))];
	}
}

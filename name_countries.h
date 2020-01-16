#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "Random.h"

// Global vector (I know, it's bad) for keeping track of available names
std::vector<std::string> taken_country_names;
std::vector<std::string> countryNames = {
	"Usnon", "Cosnaria", "Speulia", "Floaze", "Xesken", "Graela", "Astrea", "Efrye", "Fleup Fryae", "Bliuji", "Oplaecor",
	"Meskary", "Asturg", "Streay Bona", "Fushus", "Wachein", "Clae Chijan", "Plaisil", "Ieclines", "Aflurg", "Spueze", 
	"Ascen", "Pra Chal", "Pustiussau", "Koswieji", "Ecryae", "Cluyh Strana", "Ramonia", "Permen", "Hawhington", "Tosmoyssau",
	"Yuthuinga", "Swoai Whil", "Lusnax", "Shauland", "Futholand", "Agrela", "Fuspein", "Baskerland", "Rockland"
};

// Checks if the name can be found in the vector
#define is_country_name_available(name, index) std::find(taken_country_names.begin(), taken_country_names.end(), name) == taken_country_names.end()

std::string nextCountryName(Random random) {
	std::string next;
	while (countryNames.size() > 0) {
		next = countryNames[random.NextInt(0, (int)countryNames.size())];
		if (is_country_name_available(next)) {
			taken_country_names.push_back(next);
			countryNames.erase(std::remove(countryNames.begin(), countryNames.end(), next), countryNames.end());
			return next;
		}
	}
	return "LOL";
}

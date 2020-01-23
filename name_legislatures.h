#pragma once
#include <string>
#include "Random.h"

std::string name_legislatures[] = {
	"Congress", "Parliament", "National Assembly", "House of Representatives", "National Legislature", "National Council", "Supreme Legislature", "Supreme Council",
	"Legislative Assembly", "Supreme Assembly",
};

std::string GetRandomLegislatureName(Random random) {
	return name_legislatures[random.NextInt(0, sizeof(name_legislatures) / sizeof(std::string))];
}

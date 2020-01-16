#include "StringUtilities.h"
#include <sstream>
#include <iomanip>

std::string to_quantity(unsigned int quantity) {
	
	std::stringstream ss;
	std::ostringstream os;
	
	os << std::fixed;
	os << std::setprecision(2);

	if (quantity >= 1000000000) {
		double f = quantity / 1000000000.0;
		os << f;
		ss << os.str() << "B";
		return ss.str();
	} else if (quantity >= 1000000) {
		double f = quantity / 1000000.0;
		os << f;
		ss << os.str() << "M";
		return ss.str();
	} else if (quantity >= 1000) {
		double f = quantity / 1000.0;
		os << f;
		ss << os.str() << "K";
		return ss.str();
	} else {
		return std::to_string(quantity);
	}

}

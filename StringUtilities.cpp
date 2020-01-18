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

std::string get_n_suffix(int n) {
	std::string s = std::to_string(n);
	char last = s.at(s.length() - 1);
	if (last == '1') {
		return "st";
	} else if (last == '2') {
		return "nd";
	} else if (last == '3') {
		return "rd";
	} else {
		return "th";
	}
}

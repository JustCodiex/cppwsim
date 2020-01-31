#include "StringUtilities.h"
#include <sstream>
#include <iomanip>

std::string to_quantity(unsigned int quantity) {
	return to_quantity((unsigned long long)quantity);
}

std::string to_quantity(unsigned long long quantity) {

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

std::string to_string(double n, int decimals) {

	std::ostringstream os;

	os << std::fixed;
	os << std::setprecision(decimals);

	os << n;
	
	return os.str();

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

std::vector<std::string> split_string(std::string str, char character) {

	std::vector<std::string> strings;

	size_t index;
	while ((index = str.find_first_of(character)) != std::string::npos) {
		std::string sub = str.substr(0, index);
		str = str.substr(index + 1);
		strings.push_back(sub);
	}

	if (str.size() > 0) {
		strings.push_back(str);
	}

	return strings;

}

std::string to_upper(std::string str) {

	std::string s = "";

	for (size_t i = 0; i < str.size(); i++) {
		s += (char)toupper(str[i]);
	}

	return s;

}

std::string to_lower(std::string str) {

	std::string s = "";

	for (size_t i = 0; i < str.size(); i++) {
		s += (char)tolower(str[i]);
	}

	return s;

}

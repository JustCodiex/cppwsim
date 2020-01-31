#pragma once
#include <string>
#include <vector>

// Return the quantified number in string format
std::string to_quantity(unsigned int quantity);

// Return the quantified number in string format
std::string to_quantity(unsigned long long quantity);

// Return the number in a string format with decimals
std::string to_string(double n, int decimals);

// Return suffix to number
std::string get_n_suffix(int n);

// Splits the string whenever it meets specified character.
std::vector<std::string> split_string(std::string str, char character);

// Returns string in all upper case
std::string to_upper(std::string str);

// Returns string in all lower case
std::string to_lower(std::string str);

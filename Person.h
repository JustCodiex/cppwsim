#pragma once
#include "stdlib.h"

enum class Sex {
	Female,
	Male,
};

class Person {

public:

	struct Birthday {
		unsigned char month;
		unsigned char day;
		Birthday() { day = month = 1; }
		Birthday(unsigned char d, unsigned char m) { day = d; month = m; }
	};

public:

	Person();
	Person(Random random);
	Person(unsigned char age, Random random);
	Person(Sex sex, Random random);
	Person(unsigned char age, Sex sex, Random random);

	std::string GetFullName() {
		if (m_middleName != "") {
			return m_firstName + " " + m_middleName + " " + m_lastName;
		} else {
			return m_firstName + " " + m_lastName;
		}
	}

	Sex GetSex() { return m_sex; }

	unsigned char GetAge() { return m_age; }

	Birthday GetBirthday() { return m_birthday; }

	virtual std::string GetTitle() { return GetFullName(); }

private:

	Sex m_sex;
	
	unsigned char m_age;
	Birthday m_birthday;

	std::string m_firstName;
	std::string m_middleName;
	std::string m_lastName;

};

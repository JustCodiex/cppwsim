#pragma once
#include <string>

// The amount of days in each month
int day_in_months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// The name of the months
std::string month_names[12] = { 
	"January", "February", "March", "April", "May", "June", 
	"July", "August", "September", "October", "November", "December" 
};

struct TimeDate {

	int year;
	int month;
	int day;

	// Initial date January 1st, 0
	TimeDate() {
		this->year = 0;
		this->month = 1;
		this->day = 1;
	}

	// Set all details of date
	TimeDate(int year, int month, int day) {
		this->year = year;
		this->month = month;
		this->day = day;
	}

	// Advance to next day
	TimeDate nextDay() {
		TimeDate date;

		int y, m, d;
		y = this->year;
		m = this->month;
		d = this->day + 1;

		if (d > day_in_months[this->month]) {
			d = 1;
			m++;
			if (m > 12) {
				m = 1;
				y++;
			}
		}

		date.year = y;
		date.month = m;
		date.day = d;

		return date;

	}

	// Is the current date the specified date?
	bool isDate(int year, int month, int day) {
		return this->year == year && this->month == month && this->day == day;
	}

	// Get the name of the month
	std::string getNameOfMonth() {
		return month_names[this->month - 1];
	}

};

#pragma once
#include <string>
#include "Random.h"
#include "StringUtilities.h"

// The amount of days in each month
const int day_in_months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// The amount of days passed up until month
const int days_in_year_after_month[13] = {
	0,
	31,
	31 + 28,
	31 + 28 + 31,
	31 + 28 + 31 + 30,
	31 + 28 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31,
};

// The name of the months
const std::string month_names[12] = { 
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

		if (d > day_in_months[this->month-1]) {
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

	// Is the current date the given date?
	bool isDate(TimeDate date) {
		return this->year == date.year && this->month == date.month && this->day == date.day;
	}

	// Returns true if this date is later than other.
	bool isLaterThan(TimeDate other) {
		return this->getTotalDays() > other.getTotalDays();
	}

	// Returns true if this date is later or the same as other.
	bool isLaterOrSameThan(TimeDate other) {
		return this->getTotalDays() >= other.getTotalDays();
	}

	// Get the name of the month
	std::string getNameOfMonth() {
		return month_names[this->month - 1];
	}

	int getTotalDays() {
		return (this->year * 365) + days_in_year_after_month[this->month - 1] + this->day;
	}

	TimeDate addTime(TimeDate td) {
		return fromDays(this->getTotalDays() + td.getTotalDays());
	}

	TimeDate addDays(int days) {
		return fromDays(this->getTotalDays() + days);
	}

	TimeDate addYears(int years) {
		return fromDays(this->getTotalDays() + (years * 365));
	}

	std::string toString() {
		return getNameOfMonth() + " " + std::to_string(day) + get_n_suffix(day) + ", " + std::to_string(year);
	}

	// Get a random date somewhere in between the two years
	static TimeDate randomDate(int yMin, int yMax, Random random) {
		TimeDate date;
		date.year = random.NextInt(yMin, yMax);
		date.month = random.NextInt(0, 12) + 1;
		date.day = random.NextInt(0, day_in_months[date.month-1]) + 1;
		return date;
	}

	// Get a date based on the amount of days
	static TimeDate fromDays(int dayCount) {

		TimeDate date;
		int year = dayCount / 365;
		int dayofyear = dayCount % 365;

		int month = 1;
		int dayofmonth = 1;

		for (int i = 1; i < 13;i++) {
			if (dayofyear > days_in_year_after_month[i - 1] && dayofyear < days_in_year_after_month[i]) {
				month = i;
				dayofmonth = dayofyear - days_in_year_after_month[i-1];
				break;
			}
		}

		date.year = year;
		date.month = month;
		date.day = dayofmonth;

		return date;

	}

};

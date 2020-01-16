#pragma once
#include "Random.h"

class Country;

class Legislature {

public:

	struct Chamber {
		
		unsigned short seatCount;
		bool isElectable;

		bool eachStateGetsSeats;
		unsigned char eachStateSeatCount;

	};

public:

	Legislature();

	void GenerateLegislature(Country* pCountry, Random random);

	bool IsBicameral() { return m_isBicameral; }

	Chamber* GetChamber(bool firstChamber) {
		if (firstChamber) {
			return m_chambers[0];
		} else {
			return m_chambers[1];
		}
	}

private:

	bool m_isBicameral;
	Chamber* m_chambers[2];

};

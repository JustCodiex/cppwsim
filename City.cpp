#include "City.h"

City::City() {
	m_population = 0;
}

City::City(Size size, Random random) {

	if (size == Size::CITYSIZE_LARGE) {

		m_population = random.NextInt(11000, 80000);

	} else if (size == Size::CITYSIZE_METROPOLIS) {

		m_population = random.NextInt(100000, 400000);

	} else { // Small

		m_population = random.NextInt(700, 10000);

	}

}

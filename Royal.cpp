#include "Royal.h"

Royal::Royal() : Person() {
	m_popularity = 1.0f;
}

Royal::Royal(Random random) : Person(random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
}

Royal::Royal(unsigned char age, Random random) : Person(age, random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
}

Royal::Royal(Sex sex, Random random) : Person(sex, random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
}

Royal::Royal(unsigned char age, Sex sex, Random random) : Person(age, sex, random) {
	m_popularity = random.NextFloat(0.4f, 1.0f);
}

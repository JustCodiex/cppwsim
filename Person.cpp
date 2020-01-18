#include "Person.h"
#include "names_people.h"

Person::Person() {
	m_sex = Sex::Male;
	m_age = 0;
	m_firstName = "John";
	m_lastName = "Doe";
	m_middleName = "";
}

Person::Person(Random random) {
	m_sex = (random.NextBool(0.51f)) ? Sex::Female : Sex::Male;
	m_age = (unsigned char)random.NextInt(25, 71);
	m_firstName = (m_sex == Sex::Male) ? RandomMaleName(random) : RandomFemaleName(random);
	m_middleName = (random.NextBool(0.1f)) ? RandomMiddleName(random) : "";
	m_lastName = RandomLastName(random);
	m_birthday = Birthday((unsigned char)random.NextInt(1, 29), (unsigned char)random.NextInt(1, 12));
}

Person::Person(unsigned char age, Random random) {
	m_sex = (random.NextBool(0.51f)) ? Sex::Female : Sex::Male;
	m_age = age;
	m_firstName = (m_sex == Sex::Male) ? RandomMaleName(random) : RandomFemaleName(random);
	m_middleName = (random.NextBool(0.1f)) ? RandomMiddleName(random) : "";
	m_lastName = RandomLastName(random);
	m_birthday = Birthday((unsigned char)random.NextInt(1, 29), (unsigned char)random.NextInt(1, 12));
}

Person::Person(Sex sex, Random random) {
	m_sex = sex;
	m_age = (unsigned char)random.NextInt(25, 71);
	m_firstName = (m_sex == Sex::Male) ? RandomMaleName(random) : RandomFemaleName(random);
	m_middleName = (random.NextBool(0.1f)) ? RandomMiddleName(random) : "";
	m_lastName = RandomLastName(random);
	m_birthday = Birthday((unsigned char)random.NextInt(1, 29), (unsigned char)random.NextInt(1, 12));
}

Person::Person(unsigned char age, Sex sex, Random random) {
	m_sex = sex;
	m_age = age;
	m_firstName = (m_sex == Sex::Male) ? RandomMaleName(random) : RandomFemaleName(random);
	m_middleName = (random.NextBool(0.1f)) ? RandomMiddleName(random) : "";
	m_lastName = RandomLastName(random);
	m_birthday = Birthday((unsigned char)random.NextInt(1, 29), (unsigned char)random.NextInt(1, 12));
}

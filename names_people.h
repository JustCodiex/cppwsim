#pragma once
#include "stdlib.h"

const std::vector<std::string> maleNames = {
	"Henry", "Harry", "James", "John", "Jonas", "Jonathan", "Oswald", "Larry", "Percival", "Rick", "Morty", "Garry", "Allan", "Frederick", "Alex",
	"Alexander", "Thomas", "Joe", "Cameron", "David", "Neville", "Marshall", "Eric", "Harrold", "Ronald", "Martin", "Oliver", "Jack", "Fred",
	"George", "Arthur", "Uther", "Daniel", "Lucious", "Dan", "Hector", "Victor", "Michael", "Lucas", "Adam", "Christian", "Aaron", "Marcus", "Matt",
	"Matthew", "Bill", "Rasmus", "Otto", "Hans", "Jerry", "Larry", "Garry", "Gustaf", "Gus", "Noah", "Keith", "Ian", "Ianovich", "Taylor", "Thomas",
	"Tom", "Nick", "Phillip", "Carter", "Scott", "Lewis", "Mitchell", "Stewart", "Morgan", "Murphy", "Cooper", "Howard", "Edward",
};

const std::vector<std::string> femaleNames = {
	"Alexandria", "Sophia", "Tara", "Freya", "Victoria", "Monica", "Jillian", "Jill", "Elizabeth", "Margaret", "Aubrey", "Amy", "Emilty", "Emilia", "Jo",
	"Karla", "Kaya", "Kate", "Octavia", "Olivia", "Sophie", "Amanda", "Andromeda", "Vannessa", "Clarke", "Sarah", "Grace", "Rose", "Rosalina", "Guineviere",
	"Molly", "Guenevere", "Gabriella", "Isabelle", "Isabella", "Isa", "Danielle", "Ashley", "Eva", "Eva", "Rita", "Lindsey", "Agnes", "Fleur", "Ginny",
	"Hermione", "Greta", "Camille", "Camilla", "Anne", "Marie", "Anne-Marie", "Mathilda", "Marsha", "Alessia", "Alice", "Ida", "Indra", "Taylor", "Bailey",
	"Kellie", "Patricia", "Patrice", "Flora", "Laura", "Robin", "Cobie", "Lily", "Summer"
};

const std::vector<std::string> middleNames = {
	"Jean", "Reese", "", "Ostergaard", "", "Vestergaard", "Lokke", "Lunde", "", "Scavenius", "John", "", "", "Lee", "Marie", "Leah", "Alana", "Dagen", "Sheridan",
	"Deon", "Jordon", "Caden", "", "", "", "Dorien", "Adalynn", "Jaydan",
};

const std::vector<std::string> lastNames = {
	"Plaza", "Pratt", "Black", "White", "Green", "Brown", "Johnson", "Doe", "Potter", "Frederiksen", "Rasmussen", "Stein", "Steiner", "Larsson", "Shulz", "Preker", "Reichswald",
	"Stauning", "Dove", "Sturgeon", "Middleton", "Middleditch", "Longbottom", "Chamberlain", "Mozart", "Eriksen", "Ericsson", "Hansen", "Hill", "Meadows", "Baker", "Whitehall",
	"Malfoy", "Dursley", "Banks", "Diaz", "Pendragon", "Bones", "Fox", "Oliver", "Rexus", "Stirling", "Shore", "Locke", "Taylor", "Moore", "Jackson", "Thompson", "Offerman",
	"Davis", "Miller", "Wilson", "Clark", "Carter", "Nelson", "Turner", "Roberts", "Reed", "Rogers", "Bailey", "Evans", "Edwards", "Cook", "Bell", "Ward", "Howard", "Cox",
	"Parker", "Flores", "Jenkins", "Myers", "Diaz", "West", "Owens"
};

const std::string RandomMaleName(Random random) { return maleNames[random.NextInt(0, (int)maleNames.size())]; }

const std::string RandomFemaleName(Random random) { return femaleNames[random.NextInt(0, (int)femaleNames.size())]; }

const std::string RandomMiddleName(Random random) { return middleNames[random.NextInt(0, (int)middleNames.size())]; }

const std::string RandomLastName(Random random) { return lastNames[random.NextInt(0, (int)lastNames.size())]; }

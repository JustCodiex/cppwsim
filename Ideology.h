#pragma once
#include <vector>
#include <string>
#include "Random.h"

// List of all political ideologies
// registered in the project.
enum class POLITICAL_IDEOLOGY {

	ID_COMUNISM, // Traditional Communism (Marxist-Lenninism)
	ID_SYNDICALISM, // Extended Workers Right
	ID_AGRARIAN_COMMUNISM, // Chinese Communism
	ID_DEMOCRATIC_SOCIALISM, // Democratic Socialism
	ID_SOCIALISM, // Traditional Revolutionary Socialism
	ID_SOCIAL_DEMOCRACY, // Social Democracy
	ID_SOCIAL_CONSERVATISM, // Social Conservatism
	ID_NATIONAL_CONSERVATISM, // National Conservatism
	ID_ANARCHISM, // Anarchism
	ID_ANARCHO_CAPITALISM, // Anarcho-Capitalism
	ID_NEO_LIBERALISM, // Neo-Liberalism
	ID_SOCIAL_LIBERALISM, // Social-Liberalism AKA Modern Liberalism
	ID_MARKET_LIBERALISM, // Market-Liberalism
	ID_LIBERTARIAN, // Libertarian
	ID_LIBERTARIAN_SOCIALIST, // Social-Libertarian
	ID_RADICALISM, // Radicalism (Centrist)
	ID_AGRAGIANISM, // Agrarianism
	ID_ENVIORNMENTALISM, // Environmentalism
	ID_FASCISM, // Fascism
	ID_SOCIAL_NATIONALISM, // Nazism
	ID_NATIONALISM, // Nationalism
	ID_NATIONAL_LIBERALISM,
	ID_REFORMIST, // Left-Wing
	ID_REACTIONARY, // Right-Wing
	ID_CENTRISM, // Centrism

	_SIZE_

};

const std::string GetIdeologyName(POLITICAL_IDEOLOGY name);

POLITICAL_IDEOLOGY GetRandomIdeology(int y, Random random);
POLITICAL_IDEOLOGY GetRandomIdeology1800(Random random);
POLITICAL_IDEOLOGY GetRandomIdeology1850(Random random);
POLITICAL_IDEOLOGY GetRandomIdeology1870(Random random);
POLITICAL_IDEOLOGY GetRandomIdeology1900(Random random);
POLITICAL_IDEOLOGY GetRandomIdeology1920(Random random);
POLITICAL_IDEOLOGY GetRandomIdeology1960(Random random);

#define lengthof(fixedarray, type) sizeof(fixedarray) / sizeof(type)

class Ideology {

public:

	Ideology();
	Ideology(POLITICAL_IDEOLOGY ideology);

	POLITICAL_IDEOLOGY GetIdeology() { return m_indexedIdeology; }

private:

	POLITICAL_IDEOLOGY m_indexedIdeology;

};

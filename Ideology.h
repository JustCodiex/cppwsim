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

struct compass_pos {
	float libmin, libmax;
	float colmin, colmax;
	compass_pos(float libMin, float libMax, float colMin, float colMax) {
		libmin = libMin;
		libmax = libMax;
		colmin = colMin;
		colmax = colMax;
	}
};

const compass_pos compass_pos_stance[] = {
	//          lmin, lmax, cmin, cmax
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Communism, Traditional Communism (Marxist-Lenninism)
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Syndicalism, Extended Workers Right
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Agrarian Communism, Chinese Communism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Democratic Socialism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Socialism, Traditional Revolutionary Socialism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Social Democracy
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Social Conservatism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // National Conservatism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Anarchism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Anarcho-Capitalism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Neo-Liberalism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Social-Liberalism AKA Modern Liberalism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Market-Liberalism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Libertarian
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Social-Libertarian
	compass_pos(-0.1f, 0.1f, -0.1f, 0.1f), // Radicalism (Centrist)
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Agrarianism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Environmentalism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Fascism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Nazism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Nationalism
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // National Liberalism
	compass_pos(-0.3f, 0.1f, -0.3f, 0.3f), // Reformist
	compass_pos(0.1f, 1.0f, -0.1f, 1.0f), // Reactionary
	compass_pos(0.0f, 0.0f, 0.0f, 0.0f), // Centrism
};

class Ideology {

public:

	Ideology();
	Ideology(POLITICAL_IDEOLOGY ideology);

	void RegenerateStancesFromIndexedIdeology(Random random);

	void Drift(float libertarian, float collectivism);

	void UpdateIndexedIdeology();

	POLITICAL_IDEOLOGY GetIdeology() { return m_indexedIdeology; }

	const float GetLibertarianStance() { return m_libStance; }
	const float GetCollectiveStance() { return m_collectiveStance; }

	// Checks if ideology is compatible (can work with) with other ideology
	bool IsCompatible(Ideology* pIdeology);

private:

	bool IsCombination(POLITICAL_IDEOLOGY first, POLITICAL_IDEOLOGY second, POLITICAL_IDEOLOGY third, POLITICAL_IDEOLOGY fourth);

private:

	POLITICAL_IDEOLOGY m_indexedIdeology;

	float m_libStance;
	float m_collectiveStance;

};

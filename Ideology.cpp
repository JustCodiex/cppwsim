#include "Ideology.h"

Ideology::Ideology() {
    m_indexedIdeology = POLITICAL_IDEOLOGY::ID_CENTRISM;
    m_libStance = 0.0f;
    m_collectiveStance = 0.0f;
}

Ideology::Ideology(POLITICAL_IDEOLOGY ideology) {
    m_indexedIdeology = ideology;
    m_libStance = m_collectiveStance = 0.0f;
}

void Ideology::RegenerateStancesFromIndexedIdeology(Random random) {

    // Convet indexed ideology to integer
    int index = (int)m_indexedIdeology;

    // Get random values from compass
    m_libStance = random.NextFloat(compass_pos_stance[index].libmin, compass_pos_stance[index].libmax);
    m_collectiveStance = random.NextFloat(compass_pos_stance[index].colmin, compass_pos_stance[index].colmax);

}

void Ideology::Drift(float libertarian, float collectivism) {

    // Drift values
    m_libStance += libertarian;
    m_collectiveStance += collectivism;

    // Clamp values
    m_libStance = fmaxf(-1.0f, fminf(m_libStance, 1.0f));
    m_collectiveStance = fmaxf(-1.0f, fminf(m_collectiveStance, 1.0f));

    // Update indexed ideology
    this->UpdateIndexedIdeology();

}

void Ideology::UpdateIndexedIdeology() {

}

#pragma region Static Ideology Functions

const std::string GetIdeologyName(POLITICAL_IDEOLOGY name) {
	switch (name) {
    case POLITICAL_IDEOLOGY::ID_COMUNISM:
        return "Communism";
    case POLITICAL_IDEOLOGY::ID_SYNDICALISM:
        return "Syndicalism";
    case POLITICAL_IDEOLOGY::ID_AGRARIAN_COMMUNISM:
        return "Agrarian Communism";
    case POLITICAL_IDEOLOGY::ID_DEMOCRATIC_SOCIALISM:
        return "Democratic Socialism";
    case POLITICAL_IDEOLOGY::ID_SOCIALISM:
        return "Socialism";
    case POLITICAL_IDEOLOGY::ID_SOCIAL_DEMOCRACY:
        return "Social Democracy";
    case POLITICAL_IDEOLOGY::ID_SOCIAL_CONSERVATISM:
        return "Social Conservatism";
    case POLITICAL_IDEOLOGY::ID_NATIONAL_CONSERVATISM:
        return "National Conservatism";
    case POLITICAL_IDEOLOGY::ID_ANARCHISM:
        return "Anarchism";
    case POLITICAL_IDEOLOGY::ID_ANARCHO_CAPITALISM:
        return "Anarcho-Capitalism";
    case POLITICAL_IDEOLOGY::ID_NEO_LIBERALISM:
        return "Neo-Liberalism";
    case POLITICAL_IDEOLOGY::ID_SOCIAL_LIBERALISM:
        return "Social Liberalism";
    case POLITICAL_IDEOLOGY::ID_MARKET_LIBERALISM:
        return "Market Liberalism";
    case POLITICAL_IDEOLOGY::ID_LIBERTARIAN:
        return "Libertarianism";
    case POLITICAL_IDEOLOGY::ID_LIBERTARIAN_SOCIALIST:
        return "Libertarian Socialism";
    case POLITICAL_IDEOLOGY::ID_RADICALISM:
        return "Radicalism";
    case POLITICAL_IDEOLOGY::ID_AGRAGIANISM:
        return "Agrarianism";
    case POLITICAL_IDEOLOGY::ID_ENVIORNMENTALISM:
        return "Environmentalism";
    case POLITICAL_IDEOLOGY::ID_FASCISM:
        return "Fascism";
    case POLITICAL_IDEOLOGY::ID_SOCIAL_NATIONALISM:
        return "National Socialism";
    case POLITICAL_IDEOLOGY::ID_NATIONALISM:
        return "Nationalism";
    case POLITICAL_IDEOLOGY::ID_NATIONAL_LIBERALISM:
        return "National Liberalism";
    case POLITICAL_IDEOLOGY::ID_REFORMIST:
        return "Reformist";
    case POLITICAL_IDEOLOGY::ID_REACTIONARY:
        return "Reactionary";
    case POLITICAL_IDEOLOGY::ID_CENTRISM:
        return "Centrism";
    case POLITICAL_IDEOLOGY::_SIZE_:
    default:
        return std::to_string((int)POLITICAL_IDEOLOGY::_SIZE_);
	}
}

const std::string GetIdeologyName(POLITICAL_IDEOLOGY name);

POLITICAL_IDEOLOGY GetRandomIdeology1800(Random random) {
    POLITICAL_IDEOLOGY pickable[] = { POLITICAL_IDEOLOGY::ID_REACTIONARY, POLITICAL_IDEOLOGY::ID_REFORMIST, POLITICAL_IDEOLOGY::ID_RADICALISM };
    return pickable[random.NextInt(0, lengthof(pickable, POLITICAL_IDEOLOGY))];
}

POLITICAL_IDEOLOGY GetRandomIdeology1850(Random random) {
    POLITICAL_IDEOLOGY pickable[] = { 
        POLITICAL_IDEOLOGY::ID_REACTIONARY, POLITICAL_IDEOLOGY::ID_REFORMIST, POLITICAL_IDEOLOGY::ID_AGRAGIANISM,  POLITICAL_IDEOLOGY::ID_RADICALISM, POLITICAL_IDEOLOGY::ID_NATIONAL_LIBERALISM,
    };
    return pickable[random.NextInt(0, lengthof(pickable, POLITICAL_IDEOLOGY))];
}

POLITICAL_IDEOLOGY GetRandomIdeology1870(Random random) {
    POLITICAL_IDEOLOGY pickable[] = { 
        POLITICAL_IDEOLOGY::ID_REACTIONARY, POLITICAL_IDEOLOGY::ID_REFORMIST, POLITICAL_IDEOLOGY::ID_AGRAGIANISM, POLITICAL_IDEOLOGY::ID_NATIONAL_LIBERALISM,
        POLITICAL_IDEOLOGY::ID_SOCIALISM, POLITICAL_IDEOLOGY::ID_DEMOCRATIC_SOCIALISM, POLITICAL_IDEOLOGY::ID_SOCIAL_DEMOCRACY, POLITICAL_IDEOLOGY::ID_SYNDICALISM,
        POLITICAL_IDEOLOGY::ID_ANARCHISM, POLITICAL_IDEOLOGY::ID_NATIONALISM, POLITICAL_IDEOLOGY::ID_NATIONAL_CONSERVATISM,
    };
    return pickable[random.NextInt(0, lengthof(pickable, POLITICAL_IDEOLOGY))];
}

POLITICAL_IDEOLOGY GetRandomIdeology1900(Random random) {
    POLITICAL_IDEOLOGY pickable[] = {
        POLITICAL_IDEOLOGY::ID_AGRAGIANISM, POLITICAL_IDEOLOGY::ID_NATIONAL_LIBERALISM,
        POLITICAL_IDEOLOGY::ID_SOCIALISM, POLITICAL_IDEOLOGY::ID_DEMOCRATIC_SOCIALISM, POLITICAL_IDEOLOGY::ID_SOCIAL_DEMOCRACY, POLITICAL_IDEOLOGY::ID_SYNDICALISM,
        POLITICAL_IDEOLOGY::ID_ANARCHISM, POLITICAL_IDEOLOGY::ID_NATIONALISM, POLITICAL_IDEOLOGY::ID_NATIONAL_CONSERVATISM,
    };
    return pickable[random.NextInt(0, lengthof(pickable, POLITICAL_IDEOLOGY))];
}

POLITICAL_IDEOLOGY GetRandomIdeology1920(Random random) {
    POLITICAL_IDEOLOGY pickable[] = {
        POLITICAL_IDEOLOGY::ID_AGRAGIANISM, POLITICAL_IDEOLOGY::ID_NATIONAL_LIBERALISM,
        POLITICAL_IDEOLOGY::ID_SOCIALISM, POLITICAL_IDEOLOGY::ID_DEMOCRATIC_SOCIALISM, POLITICAL_IDEOLOGY::ID_SOCIAL_DEMOCRACY, POLITICAL_IDEOLOGY::ID_SYNDICALISM,
        POLITICAL_IDEOLOGY::ID_ANARCHISM, POLITICAL_IDEOLOGY::ID_NATIONALISM, POLITICAL_IDEOLOGY::ID_NATIONAL_CONSERVATISM,
    };
    return pickable[random.NextInt(0, lengthof(pickable, POLITICAL_IDEOLOGY))];
}

POLITICAL_IDEOLOGY GetRandomIdeology1960(Random random) {
    POLITICAL_IDEOLOGY pickable[] = {
        POLITICAL_IDEOLOGY::ID_AGRAGIANISM, POLITICAL_IDEOLOGY::ID_NATIONAL_LIBERALISM,
        POLITICAL_IDEOLOGY::ID_SOCIALISM, POLITICAL_IDEOLOGY::ID_DEMOCRATIC_SOCIALISM, POLITICAL_IDEOLOGY::ID_SOCIAL_DEMOCRACY, POLITICAL_IDEOLOGY::ID_SYNDICALISM,
        POLITICAL_IDEOLOGY::ID_ANARCHISM, POLITICAL_IDEOLOGY::ID_NATIONALISM, POLITICAL_IDEOLOGY::ID_NATIONAL_CONSERVATISM,
    };
    return pickable[random.NextInt(0, lengthof(pickable, POLITICAL_IDEOLOGY))];
}

POLITICAL_IDEOLOGY GetRandomIdeology(int y, Random random) {
    if (y < 1850) {
        return GetRandomIdeology1800(random);
    } else if (y < 1870) {
        return GetRandomIdeology1850(random);
    } else if (y < 1900) {
        return GetRandomIdeology1870(random);
    } else if (y < 1920) {
        return GetRandomIdeology1900(random);
    } else if (y < 1960) {
        return GetRandomIdeology1920(random);
    } else {
        return GetRandomIdeology1960(random);
    }
}

#pragma endregion

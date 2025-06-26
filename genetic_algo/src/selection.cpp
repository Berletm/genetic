#include "selection.hpp"
#include "generator.hpp"

std::vector<double> proba;
static std::random_device rd;
static std::mt19937 generator(rd());

namespace genetic
{
    Generation selection(const Generation& current_generation, std::function<Individ(const Generation&)> selection_strategy)
    {
        Generation offspring;

        while (offspring.size() != current_generation.size())
        {
            Individ individ = selection_strategy(current_generation);
            offspring.push_back(individ);
        }

        proba.clear();
        return offspring;
    }

    Individ roulette_rule(const Generation& current_generation)
    {
        if (proba.size() == 0) calculate_proba(current_generation);

        std::discrete_distribution<> dist(proba.begin(), proba.end());

        return current_generation[dist(generator)];
    }
}

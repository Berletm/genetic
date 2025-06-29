#include "mutation.hpp"
#include <random>

static std::random_device rd;
static std::mt19937 generator(rd());
static std::uniform_real_distribution udist(0.0, 1.0);

#define DELTA 0.25
#define SIGMA 1.0

namespace genetic
{
    Generation mutation(double mutation_proba, Generation& current_generation, std::function<Individ(Individ)> mutation_strategy)
    {
        Generation offspring;

        for (auto individ: current_generation) 
        {
            if (udist(generator) <= mutation_proba)
            {
                Individ mutated = mutation_strategy(individ);
                offspring.push_back(mutated);
                continue;
            }

            offspring.push_back(individ);
        }

        return offspring;
    }

    Individ swap_mutation(Individ individ)
    {
        std::uniform_int_distribution<int> idist(0, individ.size() - 1);

        int i = idist(generator), j = idist(generator);
        
        while (i == j)
        {
            i = idist(generator), j = idist(generator);
        }

        std::swap(individ[i].height, individ[j].height);

        return individ;
    }

    Individ perturbation_mutation(Individ individ)
    {
        std::uniform_real_distribution<double> perturbation_dist(-DELTA, +DELTA);
        
        for (auto& gene: individ)
        {
            gene.height += perturbation_dist(generator);
        }

        return individ;
    }

    Individ gauss_mutation(Individ individ)
    {
        std::normal_distribution<double> ndist(0, SIGMA);

        for (auto& gene: individ)
        {
            gene.height += ndist(generator);
        }

        return individ;
    }
}

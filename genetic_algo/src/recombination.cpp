#include "recombination.hpp"
#include <random>
#include <functional>
#include <algorithm>

static std::random_device rd;
static std::mt19937 generator(rd());

namespace genetic
{
    Generation recombination(double recomb_proba, Generation& current_generation, std::function<Individs(Individ, Individ)> recombination_strategy)
    {
        Generation offspring;
        
        std::uniform_int_distribution<int> idist(0, current_generation.size() - 1);
        std::uniform_real_distribution<double> pdist(0, 1); 

        while (offspring.size() != current_generation.size())
        {
            int i = idist(generator), j = idist(generator);

            if (i != j && pdist(generator) <= recomb_proba)
            {
                Individs new_individs = recombination_strategy(current_generation[i], current_generation[j]);
                
                new_individs.first.fitness = std::numeric_limits<double>::infinity();
                new_individs.second.fitness = std::numeric_limits<double>::infinity();

                offspring.push_back(new_individs.first);
                offspring.push_back(new_individs.second);
            }
        }
        
        return offspring;
    }

    Individs single_point_crossover(Individ first, Individ second)
    {
        std::uniform_int_distribution<int> dist(0, first.size() - 1);

        int crossover_point = dist(generator);

        std::swap_ranges(first.begin() + crossover_point, first.end(), second.begin() + crossover_point);

        return {first, second};
    }

    Individs arithmetic_crossover(Individ first, Individ second)
    {
        std::uniform_real_distribution<double> dist(-ALPHA_INTERVAL, 1.0f + ALPHA_INTERVAL);

        Individ first_offspring;
        Individ second_offspring;

        std::vector<double> first_alpha(first.size());
        std::vector<double> second_alpha(second.size());

        std::generate(first_alpha.begin(), first_alpha.end(), [&]() {return dist(generator);});
        std::generate(second_alpha.begin(), second_alpha.end(), [&]() {return dist(generator);});

        for (size_t i = 0; i < first.size(); ++i)
        {   
            double   height_first   = first[i].height + first_alpha[i] * (second[i].height - first[i].height);
            first_offspring.push_back(Gene{height_first, first[i].interval});

            double   height_second   = first[i].height + second_alpha[i] * (second[i].height - first[i].height);
            second_offspring.emplace_back(Gene{height_second, first[i].interval});
        }

        return {first_offspring, second_offspring};
    }
}

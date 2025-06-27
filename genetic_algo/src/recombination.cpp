#include "recombination.hpp"
#include <random>
#include <functional>

static std::random_device rd;
static std::mt19937 generator(rd());

namespace genetic
{
    Generation recombination(double recomb_proba, Generation& current_generation, std::function<Individs(Generation&)> recombination_strategy)
    {
        Generation offspring;
        
        std::uniform_int_distribution<int> idist(0, current_generation.size() - 1);
        std::uniform_real_distribution<double> pdist(0, 1); 

        while (offspring.size() != current_generation.size())
        {
            int i = idist(generator), j = idist(generator);

            if (i != j && pdist(generator) <= recomb_proba)
            {
                Individs new_individs = recombination_strategy(current_generation);
                
                offspring.push_back(new_individs.first);
                offspring.push_back(new_individs.second);
            }
        }

        return offspring;
    }

}

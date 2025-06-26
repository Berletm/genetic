#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "utils.hpp"
#include <functional>

extern std::vector<double> proba;

namespace genetic
{
    inline void measure_generation(const Polynomial& poly, Generation& current_generation)
    { for (auto& individ: current_generation) individ.fitness = fitness(poly, individ); }

    inline void calculate_proba(const Generation& current_generation)
    {
        double sum = 0; 
        for (const auto& individ: current_generation) sum += individ.fitness; 
        for (const auto& individ: current_generation) proba.push_back(1 - (individ.fitness / sum)); // q = 1 - p for minimization 
    };

    Generation selection(const Generation& current_generation, std::function<Individ(const Generation&)> selection_strategy);

    Individ    roulette_rule(const Generation& current_generation);

    Individ    scaling_rule(const Generation& current_generation);
}


#endif // SELECTION_HPP

#ifndef RECOMBINATION_HPP
#define RECOMBINATION_HPP

#include "utils.hpp"
#include <functional>

namespace genetic
{
    using Individs = std::pair<Individ, Individ>;

    Generation recombination(double recomb_proba, Generation& current_generation, std::function<Individs(Individ, Individ)> recombination_strategy); 

    Individs   single_point_crossover(Individ first, Individ second);

    Individs   arithmetic_crossover(Individ first, Individ second);
}

#endif // RECOMBINATION_HPP

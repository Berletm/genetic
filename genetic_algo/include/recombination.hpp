#ifndef RECOMBINATION_HPP
#define RECOMBINATION_HPP

#include "utils.hpp"

namespace genetic
{
    using Individs = std::pair<Individ, Individ>;

    Generation recombination(double recomb_proba, Generation& current_generation, std::function<Individs(Generation&)> recombination_strategy); 

    Individs single_point_crossover(Generation& current_generation);

    Individs arithmetic_crossover(Generation& current_generation);
}

#endif // RECOMBINATION_HPP

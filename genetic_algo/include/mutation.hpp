#ifndef MUTATION_HPP
#define MUTATION_HPP

#include "utils.hpp"
#include <functional>

#define DELTA 1.0
#define SIGMA 1.0

namespace genetic
{
    void       set_distribution(double delta, double sigma);

    Generation mutation(double mutation_proba, Generation& current_generation, std::function<Individ(Individ)> mutation_strategy, double delta = DELTA, double sigma = SIGMA);

    Individ    swap_mutation(Individ individ);

    Individ    perturbation_mutation(Individ individ);

    Individ    gauss_mutation(Individ individ);    
}


#endif // MUTATION_HPP

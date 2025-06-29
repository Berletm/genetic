#ifndef MUTATION_HPP
#define MUTATION_HPP

#include "utils.hpp"
#include <functional>

namespace genetic
{
    Generation mutation(double mutation_proba, Generation& current_generation, std::function<Individ(Individ)> mutation_strategy);

    Individ    swap_mutation(Individ individ);

    Individ    perturbation_mutation(Individ individ);

    Individ    gauss_mutation(Individ individ);    
}


#endif // MUTATION_HPP

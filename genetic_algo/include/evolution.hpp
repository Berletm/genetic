#ifndef EVOLUTION_HPP
#define EVOLUTION_HPP

#include "utils.hpp"
#include "selection.hpp"
#include "recombination.hpp"
#include "mutation.hpp"

#define EPSILON 0.005 

namespace genetic
{
    Generation evolution(
        int n_epoch,
        const Polynomial& poly,
        Generation& init_generation,
        double recombination_proba,
        double mutation_proba,
        std::function<Individ(Generation&)> selection_strategy,
        std::function<Individs(Individ, Individ)> recombination_strategy,
        std::function<Individ(Individ)> mutation_strategy,
        bool verbose = false
    );

    Individ get_best(const Generation& current_generation);
}

#endif // EVOLUTION_HPP 

#include "evolution.hpp"
#include <iostream>

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
            bool verbose)
        {
            measure_generation(poly, init_generation);

            double mean = mean_fitness(init_generation);
            double prev_mean = mean;

            Generation current_generation = init_generation;

            Generation best_generation = current_generation;

            for (size_t epoch = 0; (epoch < n_epoch) && mean >= EPSILON; ++epoch)
            {
                
                if (verbose) std::cout << mean << std::endl;

                current_generation = selection(current_generation, selection_strategy);
                current_generation = recombination(recombination_proba, current_generation, recombination_strategy);
                current_generation = mutation(mutation_proba, current_generation, mutation_strategy);
                
                measure_generation(poly, current_generation);
                mean = mean_fitness(current_generation);

                if (mean < prev_mean) best_generation = current_generation;
            }

            return best_generation;
        }
    
    Individ get_best(const Generation& current_generation)
    {
        Individ best;

        for (const auto& individ: current_generation)
        {
            if (individ.fitness < best.fitness) best = individ; 
        }

        return best;
    }
}

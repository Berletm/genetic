#include "genetic_controller.hpp"
#include "shared.hpp"
#include <evolution.hpp>

namespace genetic_gui
{
    GeneticController::GeneticController()
    {
        InitAlgo();
        poly.push_back(genetic::Monomial{1, 2});
    }

    void GeneticController::InitAlgo()
    {
        current_generation = genetic::generate_generation(
            algo_settings.generation_size, 
            algo_settings.chromosome_size, 
            algo_settings.interval
        );

        genetic::measure_generation(poly, current_generation);
        mean_fitness = genetic::mean_fitness(current_generation);
        mean_fitness_history.push_back(mean_fitness);
    }

    bool GeneticController::MakeStep()
    {
        if (current_epoch >= algo_settings.n_epoch || mean_fitness <= algo_settings.epsilon) return false;

        current_generation = genetic::evolution_step(
            poly,
            current_generation,
            algo_settings.recombination_p,
            algo_settings.mutation_p,
            algo_settings.selection_strategy,
            algo_settings.recombination_strategy,
            algo_settings.mutation_strategy);

        genetic::measure_generation(poly, current_generation);

        mean_fitness = genetic::mean_fitness(current_generation);
        mean_fitness_history.push_back(mean_fitness);

        step = genetic::get_best(current_generation);
        best_individ_history.push_back(step);

        current_epoch += 1;

        NotifyObservers();
        return true;
    }
}
#include "genetic_controller.hpp"
#include "shared.hpp"
#include <evolution.hpp>

namespace genetic_gui
{
    GeneticController::GeneticController():
    mean_fitness(0.0), 
    delta_fitness(std::numeric_limits<double>::infinity()),
    current_epoch(0),
    is_running(false)
    {}

    void GeneticController::InitAlgo()
    {   
        is_running = true;
        mean_fitness_history.clear();
        best_individ_history.clear();
        current_epoch = 0;
        mean_fitness = std::numeric_limits<double>::infinity();

        genetic::set_distribution(algo_settings.interval, poly);

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
        if (mean_fitness_history.size() >= 2) delta_fitness = mean_fitness_history[mean_fitness_history.size() - 2] - mean_fitness_history[mean_fitness_history.size() - 1]; 
        if (current_epoch >= algo_settings.n_epoch || mean_fitness <= algo_settings.epsilon)
        {
            return false;
        }

        current_generation = genetic::evolution_step(
            poly,
            current_generation,
            algo_settings.recombination_p,
            algo_settings.mutation_p,
            algo_settings.selection_strategy,
            algo_settings.recombination_strategy,
            algo_settings.mutation_strategy,
            algo_settings.delta,
            algo_settings.sigma,
            algo_settings.verbose);

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
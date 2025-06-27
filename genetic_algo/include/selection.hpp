#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "utils.hpp"
#include <functional>

namespace genetic
{
    enum class ScalingType {linear, sigma, softmax, exponential};

    inline void measure_generation(const Polynomial& poly, Generation& current_generation)
    { for (auto& individ: current_generation) individ.fitness = fitness(poly, individ); }

    inline void calculate_proba(Generation& current_generation)
    {
        double sum = 0; 
        for (const auto& individ: current_generation) sum += individ.fitness; 
        for (const auto& individ: current_generation) current_generation.proba.push_back(1 - (individ.fitness / sum)); // q = 1 - p for minimization 
    }

    Generation selection(Generation& current_generation, std::function<Individ(Generation&)> selection_strategy);

    Individ    roulette_rule(Generation& current_generation);

    void       apply_scaling(ScalingType type, Generation& current_generation);

    template <ScalingType type = ScalingType::linear>
    inline Individ scaling_rule(Generation& current_generation)
    {
        if(!current_generation.is_scaled) apply_scaling(type, current_generation);

        return roulette_rule(current_generation);
    }
}


#endif // SELECTION_HPP

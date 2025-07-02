#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "utils.hpp"
#include <functional>
#include <algorithm>

namespace genetic
{
    enum class ScalingType {linear, sigma, softmax, exponential};

    // calculating probabilities using ranks for numeric stability
    inline void calculate_proba(Generation& current_generation) 
    {
        std::sort(current_generation.begin(), current_generation.end(),
            [](const auto& a, const auto& b) { return a.fitness > b.fitness; });
        
        current_generation.proba.clear();
        const size_t n = current_generation.size();
        for (size_t i = 0; i < n; ++i) {
            current_generation.proba.push_back(static_cast<double>(i+1) / (n*(n+1)/2));
        }
    }

    Generation selection(Generation& current_generation, std::function<Individ(Generation&)> selection_strategy);

    Individ    roulette_rule(Generation& current_generation);

    void       apply_scaling(ScalingType type, Generation& current_generation, double max_after_scaling = 100.0);

    template <ScalingType type = ScalingType::linear>
    inline Individ scaling_rule(Generation& current_generation)
    {
        if(!current_generation.is_scaled) apply_scaling(type, current_generation);

        return roulette_rule(current_generation);
    }
}


#endif // SELECTION_HPP

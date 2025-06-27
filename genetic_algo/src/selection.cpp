#include "selection.hpp"
#include "generator.hpp"
#include <math.h>

static std::random_device rd;
static std::mt19937 generator(rd());
static constexpr double max_after_scaling = 100.0;

namespace genetic
{
    Generation selection(Generation& current_generation, std::function<Individ(Generation&)> selection_strategy)
    {
        Generation offspring;

        while (offspring.size() != current_generation.size())
        {
            Individ individ = selection_strategy(current_generation);
            offspring.push_back(individ);
        }

        offspring.proba.clear();
        offspring.is_scaled = false;

        return offspring;
    }

    Individ roulette_rule(Generation& current_generation)
    {
        if (current_generation.proba.size() == 0) calculate_proba(current_generation);

        std::discrete_distribution<> dist(current_generation.proba.begin(), current_generation.proba.end());

        return current_generation[static_cast<int>(dist(generator))];
    }

    void apply_scaling(ScalingType type, Generation& current_generation)
    {
        switch (type)
        {
            case (ScalingType::linear):
            {
                double max = -std::numeric_limits<double>::infinity();

                for (const auto& individ: current_generation)
                {
                    if (individ.fitness > max) max = individ.fitness;
                }
                
                double scaling_factor = max_after_scaling / max;

                for (auto& individ: current_generation)
                {
                    individ.fitness *= scaling_factor;
                }

                current_generation.is_scaled = true;
                break;
            }

            case (ScalingType::sigma):
            {
                double sum = 0;

                for (const auto& individ: current_generation)
                {
                    sum += individ.fitness;
                }

                double mean = sum / static_cast<double>(current_generation.size());

                double std = 0;

                for (const auto& individ: current_generation)
                {
                    std += pow((individ.fitness - mean), 2);
                }

                std /= static_cast<double>(current_generation.size() - 1);

                std = sqrt(std);

                for (auto& individ: current_generation)
                {
                    individ.fitness = fmax(0, 1 + ((individ.fitness - mean) / std));
                }

                current_generation.is_scaled = true;
                break;
            }

            case (ScalingType::softmax):
            {
                double sum = 0;

                for (const auto& individ: current_generation)
                {
                    sum += exp(individ.fitness);
                }

                for (auto& individ: current_generation)
                {
                    individ.fitness = exp(individ.fitness) / sum;
                }

                current_generation.is_scaled = true;
                break;
            }

            case (ScalingType::exponential):
            {
                for (auto& individ: current_generation)
                {
                    individ.fitness = exp(individ.fitness);
                }

                current_generation.is_scaled = true;
                break;
            }
        }
    }
}

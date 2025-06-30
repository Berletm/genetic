#include "shared.hpp"

namespace genetic_gui
{
    RenderingSettings   render_settings;
    AlgoSettings        algo_settings;
    genetic::Polynomial poly;
    genetic::Individ    step;

    std::vector<double>           mean_fitness_history;
    std::vector<genetic::Individ> best_individ_history;
}

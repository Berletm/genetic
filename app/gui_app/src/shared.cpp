#include "shared.hpp"
#include "genetic_frame.hpp"

namespace genetic_gui
{   
    void initialize_globals(const genetic::Polynomial& initial_poly, const AlgoSettings& settings)
    {
        poly = initial_poly;
        algo_settings = settings;
    }

    Frames              frames;
    AlgoSettings        algo_settings;
    RenderingSettings   render_settings;
    genetic::Polynomial poly;
    genetic::Individ    step;

    std::vector<double>           mean_fitness_history;
    std::vector<genetic::Individ> best_individ_history;
}

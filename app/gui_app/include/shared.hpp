#ifndef SHARED_HPP
#define SHARED_HPP

#include <utils.hpp>
#include <selection.hpp>
#include <recombination.hpp>
#include <mutation.hpp>
#include <wx-3.2/wx/glcanvas.h>

#define X_MAX 5
#define X_MIN -X_MAX
#define Y_MAX 5
#define Y_MIN -Y_MAX

namespace genetic_gui 
{
    struct RenderingSettings
    {
        double x_max = X_MAX, x_min = X_MIN;
        double y_max = Y_MAX, y_min = Y_MIN;
        bool   show_legend = true;
        int    resolution = 500;
        int    gl_attribs[3] = {WX_GL_SAMPLES, 4, 0};
    };

    struct AlgoSettings
    {
        genetic::Interval interval = {X_MIN, X_MAX};
        int n_epoch = 500, generation_size = 100, chromosome_size = 100;
        
        double mutation_p = 0.2, recombination_p = 0.7, epsilon = EPSILON;

        std::function<genetic::Individ(genetic::Generation&)> selection_strategy = genetic::scaling_rule<genetic::ScalingType::sigma>;
        std::function<genetic::Individs(genetic::Individ, genetic::Individ)> recombination_strategy = genetic::arithmetic_crossover;
        std::function<genetic::Individ(genetic::Individ)> mutation_strategy = genetic::swap_mutation;
    };

    extern RenderingSettings   render_settings;
    extern AlgoSettings        algo_settings;
    extern genetic::Polynomial poly;
    extern genetic::Individ    step;

    extern std::vector<double>           mean_fitness_history;
    extern std::vector<genetic::Individ> best_individ_history;
}

#endif // SHARED_HPP 

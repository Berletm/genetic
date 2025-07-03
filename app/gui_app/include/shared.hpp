#ifndef SHARED_HPP
#define SHARED_HPP

#include <utils.hpp>
#include <selection.hpp>
#include <recombination.hpp>
#include <mutation.hpp>
#include <wx-3.2/wx/glcanvas.h>
#include <evolution.hpp>
#include <unordered_map>
#include "settings_frames.hpp"
#include "file_frames.hpp"

#define X_MAX 15
#define X_MIN -X_MAX
#define Y_MAX 15
#define Y_MIN -Y_MAX

namespace genetic_gui 
{
    static std::unordered_map<genetic::SelectionMethod, std::function<genetic::Individ(genetic::Generation&)>> selection_map = 
    {
        {genetic::SelectionMethod::Roulette, genetic::roulette_rule},
        {genetic::SelectionMethod::LinearScaling, genetic::scaling_rule<genetic::ScalingType::linear>},
        {genetic::SelectionMethod::SigmaScaling, genetic::scaling_rule<genetic::ScalingType::sigma>},
        {genetic::SelectionMethod::SoftmaxScaling, genetic::scaling_rule<genetic::ScalingType::softmax>},
        {genetic::SelectionMethod::ExponentialScaling, genetic::scaling_rule<genetic::ScalingType::exponential>}
    };

    static std::unordered_map<genetic::RecombinationMethod, std::function<genetic::Individs(genetic::Individ, genetic::Individ)>> recombination_map = 
    {
        {genetic::RecombinationMethod::SinglePoint, genetic::single_point_crossover},
        {genetic::RecombinationMethod::Arithmetic, genetic::arithmetic_crossover}
    };

    static std::unordered_map<genetic::MutationMethod, std::function<genetic::Individ(genetic::Individ)>> mutation_map =
    {
        {genetic::MutationMethod::Swap, genetic::swap_mutation},
        {genetic::MutationMethod::Gauss, genetic::gauss_mutation},
        {genetic::MutationMethod::Perturbation, genetic::perturbation_mutation}
    };

    class GeneticFrame;

    enum 
    {
        ID_New = 7777,
        ID_Load,
        ID_Save,
        ID_Main,
        ID_AlgoSettings,
        ID_RendSettings,
        ID_Next,
        ID_Prev,
        ID_Stop
    };

    struct RenderingSettings
    {
        double x_max = X_MAX, x_min = X_MIN;
        double y_max = Y_MAX, y_min = Y_MIN;
        bool   show_legend = true;
        int    resolution = 10;
        int    fps = 15;
        int    gl_attribs[3] = {WX_GL_SAMPLES, 4, 0};
    };

    struct AlgoSettings
    {
        genetic::Interval interval = {X_MIN, X_MAX};
        int n_epoch = 1000, generation_size = 300, chromosome_size = 100;
        
        double mutation_p = 0.1, recombination_p = 0.7, epsilon = EPSILON;

        genetic::SelectionMethod     selection_id;
        genetic::RecombinationMethod recombination_id;
        genetic::MutationMethod      mutation_id;

        std::function<genetic::Individ(genetic::Generation&)> selection_strategy;
        std::function<genetic::Individs(genetic::Individ, genetic::Individ)> recombination_strategy;
        std::function<genetic::Individ(genetic::Individ)> mutation_strategy;

        bool verbose = true;

        double max_after_scaling = 100;
        double delta = 1.0, sigma = 1.0;
    };

    struct Frames
    {
        GeneticFrame      *mainFrame;
        NewFrame          *newFrame;
        AlgoSettingsFrame *algoFrame;
        RendSettingsFrame *rendFrame;
    };

    struct HistoryState 
    {
        genetic::Generation           generation;
        double                        fitness;
        genetic::Individ              best_individ;
    };

    extern Frames              frames;
    extern RenderingSettings   render_settings;
    extern AlgoSettings        algo_settings;
    extern genetic::Polynomial poly;
    extern genetic::Individ    step;

    extern std::vector<double>           mean_fitness_history;
    extern std::vector<genetic::Individ> best_individ_history;
}

#endif // SHARED_HPP 

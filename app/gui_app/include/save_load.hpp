#ifndef SAVE_LOAD_HPP
#define SAVE_LOAD_HPP

#include "json.hpp"
#include "genetic_controller.hpp"
#include "shared.hpp"

using json = nlohmann::json;

namespace nlohmann
{
    // Gene
    template <>
    struct adl_serializer<genetic::Gene> 
    {
        static void to_json(json& j, const genetic::Gene& gene) 
        {
            j = json
            {
                {"interval", {gene.interval.first, gene.interval.second}},
                {"height", gene.height}
            };
        }

        static void from_json(const json& j, genetic::Gene& gene) 
        {
            gene.interval = j.at("interval").get<std::pair<double, double>>();
            gene.height = j.at("height").get<double>();
        }
    };

    // Individ
    template<>
    struct adl_serializer<genetic::Individ>
    {
        static void to_json(json& j, const genetic::Individ& individ)
        {
            j = json
            {
                {"genes", static_cast<const std::vector<genetic::Gene>&>(individ)},
                {"fitness", individ.fitness}
            };
        }

        static void from_json(const json& j, genetic::Individ& individ)
        {
            auto& genes = static_cast<std::vector<genetic::Gene>&>(individ);
            genes = j.at("genes").get<std::vector<genetic::Gene>>();
            individ.fitness = j.at("fitness").get<double>();
        }
    };

    // Generation
    template<>
    struct adl_serializer<genetic::Generation>
    {
        static void to_json(json& j, const genetic::Generation& generation)
        {
            j = json
            {
                {"generation", static_cast<const std::vector<genetic::Individ>&>(generation)},
                {"proba", generation.proba},
                {"is_scaled", generation.is_scaled}
            };
        }

        static void from_json(const json& j, genetic::Generation& generation)
        {
            auto& individs = static_cast<std::vector<genetic::Individ>&>(generation);
            individs = j.at("generation").get<std::vector<genetic::Individ>>();
            generation.is_scaled = j.at("is_scaled").get<bool>();
            generation.proba = j.at("proba").get<std::vector<double>>();
        }
    };

    // AlgoSettings
    template<>
    struct adl_serializer<genetic_gui::AlgoSettings>
    {
        static void to_json(json& j, const genetic_gui::AlgoSettings& algo_settings_)
        {
            j = json
            {
                {"interval", algo_settings_.interval},
                {"n_epoch", algo_settings_.n_epoch},
                {"generation_size", algo_settings_.generation_size},
                {"chromosome_size", algo_settings_.chromosome_size},
                {"mutation_p", algo_settings_.mutation_p},
                {"recombination_p", algo_settings_.recombination_p},
                {"epsilon", algo_settings_.epsilon},
                {"selection_id", algo_settings_.selection_id},
                {"recombination_id", algo_settings_.recombination_id},
                {"mutation_id", algo_settings_.mutation_id},
                {"verbose", algo_settings_.verbose},
                {"max_after_scaling", algo_settings_.max_after_scaling},
                {"delta", algo_settings_.delta},
                {"sigma", algo_settings_.sigma}
            };
        }

        static void from_json(const json& j, genetic_gui::AlgoSettings& algo_settings_)
        {
            algo_settings_.interval = j.at("interval").get<std::pair<double, double>>();
            algo_settings_.n_epoch = j.at("n_epoch").get<int>();
            algo_settings_.generation_size = j.at("generation_size").get<int>();
            algo_settings_.chromosome_size = j.at("chromosome_size").get<int>();
            algo_settings_.mutation_p = j.at("mutation_p").get<double>();
            algo_settings_.recombination_p = j.at("recombination_p").get<double>();
            algo_settings_.epsilon = j.at("epsilon").get<double>();
            
            algo_settings_.selection_id = j.at("selection_id").get<genetic::SelectionMethod>();
            algo_settings_.recombination_id = j.at("recombination_id").get<genetic::RecombinationMethod>();
            algo_settings_.mutation_id = j.at("mutation_id").get<genetic::MutationMethod>();

            algo_settings_.selection_strategy     = genetic_gui::selection_map[algo_settings_.selection_id];
            algo_settings_.recombination_strategy = genetic_gui::recombination_map[algo_settings_.recombination_id];
            algo_settings_.mutation_strategy      = genetic_gui::mutation_map[algo_settings_.mutation_id];

            algo_settings_.verbose = j.at("verbose").get<bool>();
            algo_settings_.max_after_scaling = j.at("max_after_scaling").get<double>();
            algo_settings_.delta = j.at("delta").get<double>();
            algo_settings_.sigma = j.at("sigma").get<double>();
        }
    };

    // RenderingSettings
    template<>
    struct adl_serializer<genetic_gui::RenderingSettings>
    {
         static void to_json(json& j, const genetic_gui::RenderingSettings& rend_settings_)
        {
            j = json
            {
                {"x_max", rend_settings_.x_max},
                {"x_min", rend_settings_.x_min},
                {"y_max", rend_settings_.y_max},
                {"y_min", rend_settings_.y_min},
                {"show_legend", rend_settings_.show_legend},
                {"resolution", rend_settings_.resolution},
            };
        }

        static void from_json(const json& j, genetic_gui::RenderingSettings& rend_settings_)
        {
            rend_settings_.x_max = j.at("x_max").get<double>();
            rend_settings_.x_min = j.at("x_min").get<double>();
            rend_settings_.y_max = j.at("y_max").get<double>();
            rend_settings_.y_min = j.at("y_min").get<double>();
            rend_settings_.show_legend = j.at("show_legend").get<bool>();
            rend_settings_.resolution  = j.at("resolution").get<int>();
        }
    };


    // GeneticController
    template<>
    struct adl_serializer<genetic_gui::GeneticController>
    {
        static void to_json(json& j, const genetic_gui::GeneticController& controller)
        {
            j = json
            {
                {"current_generation", controller.current_generation},
                {"current_epoch", controller.current_epoch},
                {"mean_fitness", controller.mean_fitness}
            };
        }

        static void from_json(const json& j, genetic_gui::GeneticController& controller)
        {
            controller.current_generation = j.at("current_generation").get<genetic::Generation>();
            controller.mean_fitness = j.at("mean_fitness").get<double>();
            controller.current_epoch = 0;
        }
    };

    // Monomial
    template<>
    struct adl_serializer<genetic::Monomial>
    {
        static void to_json(json& j, const genetic::Monomial& mono)
        {
            j = json
            {
                {"coefficient", mono.coefficient},
                {"power", mono.power}
            };
        }

        static void from_json(const json& j, genetic::Monomial& mono)
        {
            mono.coefficient = j.at("coefficient").get<double>();
            mono.power = j.at("power").get<double>();
        }
    };

    // Polynomial
    template<>
    struct adl_serializer<genetic::Polynomial>
    {
        static void to_json(json& j, const genetic::Polynomial& poly)
        {
            j = json
            {
                {"monomials", static_cast<const std::vector<genetic::Monomial>>(poly)}
            };
        }

        static void from_json(const json& j, genetic::Polynomial& poly)
        {
            auto& monomials = static_cast<std::vector<genetic::Monomial>&>(poly);
            monomials = j.at("monomials").get<std::vector<genetic::Monomial>>();
        }
    };
}

#endif // SAVE_LOAD_HPP
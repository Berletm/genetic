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
            auto interval = j.at("interval").get<std::pair<double, double>>();
            gene.interval = interval;
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
            static_cast<std::vector<genetic::Gene>&>(individ) = j.at("genes").get<std::vector<genetic::Gene>>();
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
            static_cast<std::vector<genetic::Individ>&>(generation) = j.at("generation").get<std::vector<genetic::Individ>>();
            generation.is_scaled = j.at("is_scaled").get<bool>();
            generation.proba = j.at("proba").get<std::vector<double>>();
        }
    };

    // HistoryState
    template<>
    struct adl_serializer<genetic_gui::HistoryState>
    {
        static void to_json(json& j, const genetic_gui::HistoryState& hs)
        {
            j = json
            {
                {"generation", hs.generation},
                {"fitness", hs.fitness},
                {"best_individ", hs.best_individ}
            };
        }
        
        static void from_json(const json& j, genetic_gui::HistoryState& hs)
        {
            j.at("generation").get_to(hs.generation);
            j.at("best_individ").get_to(hs.best_individ);
            j.at("fitness").get_to(hs.fitness);
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
            j.at("interval").get_to(algo_settings_.interval);
            j.at("n_epoch").get_to(algo_settings_.n_epoch);
            j.at("generation_size").get_to(algo_settings_.generation_size);
            j.at("chromosome_size").get_to(algo_settings_.chromosome_size);
            j.at("mutation_p").get_to(algo_settings_.mutation_p);
            j.at("recombination_p").get_to(algo_settings_.recombination_p);
            j.at("epsilon").get_to(algo_settings_.epsilon);
            j.at("selection_id").get_to(algo_settings_.selection_id);
            j.at("recombination_id").get_to(algo_settings_.recombination_id);
            j.at("mutation_id").get_to(algo_settings_.mutation_id);
            j.at("verbose").get_to(algo_settings_.verbose);
            j.at("max_after_scaling").get_to(algo_settings_.max_after_scaling);
            j.at("delta").get_to(algo_settings_.delta);
            j.at("sigma").get_to(algo_settings_.sigma);
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
                {"mean_fitness", controller.mean_fitness},
                {"delta_fitness", controller.delta_fitness},
                {"is_running", controller.is_running},
                {"undo_history", controller.undo_history},
                {"redo_history", controller.redo_history},
                {"current_epoch", controller.current_epoch}
            };
        }

        static void from_json(const json& j, genetic_gui::GeneticController& controller)
        {
            j.at("current_generation").get_to(controller.current_generation);
            j.at("mean_fitness").get_to(controller.mean_fitness);
            j.at("delta_fitness").get_to(controller.delta_fitness);
            j.at("is_running").get_to(controller.is_running);
            j.at("undo_history").get_to(controller.undo_history);
            j.at("redo_history").get_to(controller.redo_history);
            j.at("current_epoch").get_to(controller.current_epoch);
        }
    };
}

#endif // SAVE_LOAD_HPP

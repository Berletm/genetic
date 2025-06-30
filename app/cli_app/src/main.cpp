#include <generator.hpp>
#include <evolution.hpp>

#include <iostream>

genetic::Polynomial poly;

static double left, right;

static int chromosome_size = 300, generation_size = 500;

static double recomb_proba = 0.7, mutation_proba = 0.2;

static int n_epoch = 2000;

int main(int argc, char* argv[])
{
    poly.push_back({1, 2});

    if (argc > 2)
    {
        left  = atof(argv[1]);
        right = atof(argv[2]);       
    }

    genetic::set_distribution({left, right}, poly);

    genetic::Generation generation;
    generation = genetic::generate_generation(generation_size, chromosome_size, {left, right});

    genetic::Generation best_generation = genetic::evolution(
        n_epoch, 
        poly, generation,
        recomb_proba,
        mutation_proba,
        genetic::scaling_rule<genetic::ScalingType::sigma>,
        genetic::arithmetic_crossover,
        genetic::swap_mutation,
        true
    );

    genetic::Individ best_individ = genetic::get_best(best_generation);
    
    for (const auto& gene: best_individ)
    {
        std::cout << gene.height << " [" << gene.interval.first << ";" << gene.interval.second << "]" << std::endl;
    }

    return 0;
}

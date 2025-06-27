#include <generator.hpp>
#include <selection.hpp>
#include <iostream>

genetic::Polynomial poly;

static double left, right;

static int chromosome_size = 10, generation_size = 10;

int main(int argc, char* argv[])
{
    poly.push_back({1, 2});

    if (argc > 2)
    {
        left  = atof(argv[1]);
        right = atof(argv[2]);       
    }

    genetic::Partition intervals;

    intervals = genetic::interval_partition(chromosome_size, left, right);

    genetic::Generation generation;
    generation = genetic::generate_generation(generation_size, chromosome_size, intervals);
    
    genetic::measure_generation(poly, generation);
    genetic::Generation offspring = genetic::selection(generation, genetic::scaling_rule<>);

    for (const auto& individ: offspring)
    {
        std::cout << individ.fitness << std::endl;
    }
    
    return 0;
}

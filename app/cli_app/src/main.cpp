#include <generator.hpp>
#include <iostream>

static double left, right;

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        left  = atof(argv[1]);
        right = atof(argv[2]);       
    }

    genetic::Partition intervals;

    intervals = genetic::interval_partition(left, right);

    genetic::Individ individ = genetic::generate_individ(intervals);

    for (const auto& gene: individ.chromosome)
    {
        std::cout << gene.height << std::endl;
    }
    return 0;
}
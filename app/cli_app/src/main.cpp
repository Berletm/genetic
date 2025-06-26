#include <generator.hpp>
#include <iostream>

genetic::Polynomial poly;

static double left, right;

int main(int argc, char* argv[])
{
    poly.poly.push_back({1, 2});

    if (argc > 2)
    {
        left  = atof(argv[1]);
        right = atof(argv[2]);       
    }

    genetic::Partition intervals;

    intervals = genetic::interval_partition(left, right);

    genetic::Individ individ = genetic::generate_individ(intervals);

    std::cout << genetic::fitness(poly, individ) << std::endl;
    
    return 0;
}
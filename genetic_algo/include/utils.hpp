#ifndef UTILS_HPP
#define UTILS_HPP

#include <utility>
#include <vector>

#define MAX_HEIGHT 20
#define MIN_HEIGHT -MAX_HEIGHT

#define CHROMOSOME_SIZE 10
#define GENERATION_SIZE 100

namespace genetic
{
    struct Gene
    {
        double height;
        std::pair<double, double> interval;
    };

    struct Individ
    {
        std::vector<Gene> chromosome;

        // crossover operator
        Individ operator+(const Individ &other);
    };

    using Generation = std::vector<Individ>;

    using Interval   = std::pair<double, double>;
    using Partition  = std::vector<Interval>;
}

#endif // UTILS_HPP

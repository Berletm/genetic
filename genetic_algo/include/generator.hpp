#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "utils.hpp"
#include <random>

#define INIT_UNIFORM 10.0

namespace genetic
{
    void set_distribution(const Interval& interval, const Polynomial& poly);

    Partition interval_partition(const int chromosome_size, Interval interval); 

    Gene generate_gene(const Interval& interval);

    Individ generate_individ(const int chromosome_size, const Partition& interval_partition);
    
    Generation generate_generation(const int generation_size, const int chromosome_size, const Interval& interval);
}

#endif // GENERATOR_HPP

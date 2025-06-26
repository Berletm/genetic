#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "utils.hpp"
#include <random>

namespace genetic
{
    Partition interval_partition(const int chromosome_size, double left, double right); 

    Gene generate_gene(const Interval& interval);

    Individ generate_individ(const int chromosome_size, const Partition& interval_partition);
    
    Generation generate_generation(const int generation_size, const int chromosome_size, const Partition& interval_partition);
}

#endif // GENERATOR_HPP

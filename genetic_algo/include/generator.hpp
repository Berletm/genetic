#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "utils.hpp"

namespace genetic
{
    Partition interval_partition(double left, double right); 

    Gene generate_gene(const Interval& interval);

    Individ generate_individ(const Partition& interval_partition);
    
    Generation generate_generation(const Partition& interval_partition);
}

#endif // GENERATOR_HPP

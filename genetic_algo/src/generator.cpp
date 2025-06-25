#include "generator.hpp"
#include <random>

static std::random_device rd; // gives seed for random can be replaced with param
static std::mt19937 generator(rd());
static std::uniform_real_distribution<double> udist(MIN_HEIGHT, MAX_HEIGHT);

namespace genetic
{
    Partition interval_partition(double left, double right)
    {
        double interval_length = right - left;
        
        double delta = interval_length / static_cast<double>(CHROMOSOME_SIZE);

        Partition intervals;
        while (left + delta < right)
        {
            Interval current_interval = {left, left + delta};
            intervals.push_back(current_interval);
            left += delta;
        }

        return intervals;
    }

    Gene generate_gene(const Interval& interval)
    {
        double height = udist(generator);
        
        return Gene{height, interval};
    }

    Individ generate_individ(const Partition& interval_partition)
    {
        Individ individ;

        for (size_t i = 0; i < CHROMOSOME_SIZE; ++i)
        {
            Gene gene = generate_gene(interval_partition[i]);
            individ.chromosome.push_back(gene);
        }

        return individ;
    }

    Generation generate_generation(const Partition& interval_partition)
    {
        Generation gen;

        for (size_t i = 0; i < GENERATION_SIZE; ++i)
        {
            Individ individ = generate_individ(interval_partition);
            gen.push_back(individ);
        }

        return gen;
    }
}

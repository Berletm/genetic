#include "generator.hpp"

static std::random_device rd; // gives seed for random can be replaced with param
static std::mt19937 generator(rd());
static std::uniform_real_distribution<double> udist(MIN_HEIGHT, MAX_HEIGHT);

namespace genetic
{
    Partition interval_partition(const int chromosome_size, double left, double right)
    {
        double interval_length = right - left;
        
        double delta = interval_length / static_cast<double>(chromosome_size);

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

    Individ generate_individ(const int chromosome_size, const Partition& interval_partition)
    {
        Individ individ;

        for (size_t i = 0; i < chromosome_size; ++i)
        {
            Gene gene = generate_gene(interval_partition[i]);
            individ.chromosome.push_back(gene);
        }

        return individ;
    }

    Generation generate_generation(const int generation_size, const int chromosome_size, const Partition& interval_partition)
    {
        Generation gen;

        for (size_t i = 0; i < generation_size; ++i)
        {
            Individ individ = generate_individ(chromosome_size, interval_partition);
            gen.push_back(individ);
        }

        return gen;
    }
}

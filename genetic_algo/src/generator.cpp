#include "generator.hpp"

static std::random_device rd; // gives seed for random can be replaced with param
static std::mt19937 generator(rd());
static std::uniform_real_distribution<double> udist(-INIT_UNIFORM, +INIT_UNIFORM);

namespace genetic
{
    void set_distribution(const Interval& interval, const Polynomial& poly)
    {
        double bound = std::max(abs(poly.eval(interval.first)), abs(poly.eval(interval.second))); 
        udist = std::uniform_real_distribution<double>(-bound, +bound);
    }

    Partition interval_partition(const int chromosome_size, Interval interval)
    {
        double interval_length = interval.second - interval.first;
        
        double delta = interval_length / static_cast<double>(chromosome_size);

        Partition intervals;
        
        for (size_t i = 0; (i < chromosome_size) || (interval.first + delta <= interval.second); ++i)
        {
            intervals.emplace_back(Interval{interval.first, interval.first + delta});
            interval.first += delta;
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
            individ.push_back(gene);
        }

        return individ;
    }

    Generation generate_generation(const int generation_size, const int chromosome_size, const Interval& interval)
    {
        Generation gen;

        Partition partition = interval_partition(chromosome_size, interval);

        for (size_t i = 0; i < generation_size; ++i)
        {
            Individ individ = generate_individ(chromosome_size, partition);
            gen.push_back(individ);
        }

        return gen;
    }
}

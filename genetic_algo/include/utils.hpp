#ifndef UTILS_HPP
#define UTILS_HPP

#include <utility>
#include <vector>
#include <limits>
#include <math.h>

#define ALPHA_INTERVAL 0.25f

namespace genetic
{
    struct Gene
    {
        double height;
        std::pair<double, double> interval;
    };

    struct Individ: public std::vector<Gene>
    {
        double fitness = std::numeric_limits<double>::infinity();
    };

    struct Generation: public std::vector<Individ>
    {   
        bool is_scaled = false;
        std::vector<double> proba;
    };
    
    using Interval   = std::pair<double, double>;
    using Partition  = std::vector<Interval>;

    struct Monomial {double coefficient = 1, power = 0;};

    struct Polynomial: public std::vector<Monomial>
    {
        inline double eval(double x) const
        {
            double res = 0;

            for (const auto& monomial: *this) res += monomial.coefficient * pow(x, monomial.power);

            return res;
        };
    };

    inline double fitness(const Polynomial& poly, const Individ& individ)
    {
        double error = 0;

        for (const auto& gene: individ)
        {
            double target_f_mean = (poly.eval(gene.interval.first) + poly.eval(gene.interval.second)) / 2;
            
            double delta = abs(target_f_mean - gene.height); // maybe try pow(..., 2)
            
            error += delta;
        }

        return error;
    }

    inline void measure_generation(const Polynomial& poly, Generation& current_generation)
    { for (auto& individ: current_generation) individ.fitness = fitness(poly, individ); }

    inline double mean_fitness(const Generation& current_generation)
    {
        double sum = 0;
        for (const auto& individ: current_generation) sum += individ.fitness;
        return (sum / current_generation.size());
    }
}

#endif // UTILS_HPP

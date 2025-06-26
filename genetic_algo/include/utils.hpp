#ifndef UTILS_HPP
#define UTILS_HPP

#include <utility>
#include <vector>
#include <math.h>

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

    struct Polynomial
    {
        struct Monomial {double coefficient = 1, power;};

        std::vector<Monomial> poly;

        inline double eval(double x) const
        {
            double res = 0;

            for (const auto& monomial: poly)
            {
                res += monomial.coefficient * pow(x, monomial.power);
            }

            return res;
        };
    };

    inline double fitness(const Polynomial& poly, const Individ& individ)
    {
        double error = 0;

        for (const auto& gene: individ.chromosome)
        {
            double target_f_mean = (poly.eval(gene.interval.first) + poly.eval(gene.interval.second)) / 2;
            
            double delta = abs(target_f_mean - gene.height); // maybe try pow(..., 2)
            
            error += delta;
        }

        return error;
    }
}

#endif // UTILS_HPP

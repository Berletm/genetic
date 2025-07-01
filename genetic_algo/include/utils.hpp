#ifndef UTILS_HPP
#define UTILS_HPP

#include <utility>
#include <vector>
#include <limits>
#include <math.h>

#define ALPHA_INTERVAL 0.25f
#define EPSILON 0.00001 

namespace genetic
{   
    enum class SelectionMethod
    {
        Roulette,
        LinearScaling,
        SigmaScaling,
        SoftmaxScaling,
        ExponentialScaling
    };

    enum class RecombinationMethod
    {
        SinglePoint,
        Arithmetic
    };

    enum class MutationMethod 
    {
        Swap,
        Gauss,
        Perturbation
    };

    struct Gene
    {
        double height;
        std::pair<double, double> interval;
    };

    struct Individ: public std::vector<Gene>
    {
        double fitness = std::numeric_limits<double>::infinity();

        inline double eval(double x) const
        {
            int left = 0;
            int right = this->size() - 1;

            while (left <= right)
            {
                int mid = (left + right) / 2;
                
                const auto& interval = this->at(mid);

                if (x >= interval.interval.first && x <= interval.interval.second)
                {
                    return this->at(mid).height;
                }
                else if (x < interval.interval.first)
                {
                    right = mid - 1;
                }
                else
                {
                    left = mid + 1;
                }
            }

            return 0.0;
        }
    };

    struct Generation: public std::vector<Individ>
    {   
        bool is_scaled = false;
        std::vector<double> proba;
    };
    
    using Interval   = std::pair<double, double>;
    using Partition  = std::vector<Interval>;
    using Individs = std::pair<Individ, Individ>;

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

    inline double fitness(const Polynomial& poly, const Individ& individ, double step)
    {
        double error = 0;

        for (double x = individ.front().interval.first; x < individ.back().interval.second; x += step)
        {
            double target_f = poly.eval(x);

            double approx_f = individ.eval(x);

            double delta = abs(target_f - approx_f); // maybe try pow(..., 2)
            
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

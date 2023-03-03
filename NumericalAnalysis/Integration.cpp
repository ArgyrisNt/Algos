#include <iostream>
#include <random>
#include <cassert>

namespace NumericalAnalysis
{

    double Integration::MonteCarlo(int (*func)(double, double), double min_x, double max_x, double min_y, double max_y)
    {
        std::default_random_engine generator;
        std::uniform_real_distribution<double> distribution_x(min_x, max_x);
        std::uniform_real_distribution<double> distribution_y(min_y, max_y);

        int sum = 0.0;
        int N = 9999;
        for (int i = 0; i < N; ++i)
        {
            double rand_x = distribution_x(generator);
            double rand_y = distribution_y(generator);
            sum += func(rand_x, rand_y);
        }

        double area = (max_x - min_x) * (max_y - min_y);

        return (area / (double) N) * (double) sum;
    }

    double Integration::GaussLegendre(double (*func)(double), double a, double b, int N)
    {
        assert(N > 0);
        if (N > 5) N = 5;
        
        std::vector<double> points;
        std::vector<double> weights;
        
        switch (N)
        {
        case 1:
            points = {0.0};
            weights = {2.0};
            break;
        case 2:
            points = {-0.57735, 0.57735};
            weights = {1.0, 1.0};
            break;
        case 3:
            points = {0.0, -0.774597, 0.774597};
            weights = {0.888889, 0.555556, 0.555556};
            break;
        case 4:
            points = {-0.861136, -0.339981, 0.339981, 0.861136};
            weights = {0.347855, 0.652145, 0.652145, 0.347855};
            break;
        case 5:
            points = {-0.90618, -0.538469, 0.0, 0.538469, 0.90618};
            weights = {0.236927, 0.478629, 0.568889, 0.478629, 0.236927};
            break;
        default:
            std::cout << "Invalid dimension. Valid dimensions are 1,2,3,4,5." << std::endl;
            throw std::invalid_argument("Invalid dimension");
            break;
        }

        std::vector<double> GS_pts;
        for (int i = 0; i < points.size(); i++)
        {
            GS_pts.push_back((a * (1 - points[i]) + b * (1 + points[i])) / 2);
        }

        double sum = 0.0;
        for (int i = 0; i < GS_pts.size(); i++)
        {
            sum += weights[i] * func(GS_pts[i]);
        }

        return sum;
    }

}
#include <iostream>

namespace NumericalAnalysis
{

    class Integration
    {
    public:
        double MonteCarlo(int (*func)(double, double), double min_x, double max_x, double min_y, double max_y);
        double GaussLegendre(double (*func)(double), double a, double b, int N);
    };

} // namespace NumericalAnalysis

#include "Integration.cpp"

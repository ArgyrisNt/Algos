// Example of numerical integration algorithms

#include <iostream>
#include "..\NumericalAnalysis\Integration.h"

int f(double x, double y)
{
    if (x * x + y * y < 1.0) return 1;

    return 0;
}

double g(double x)
{
    return x;
}

int main()
{
    NumericalAnalysis::Integration integration;
    double res = integration.MonteCarlo(f, -1.0, 1.0, -1.0, 1.0);
    std::cout << res << std::endl;

    res = integration.GaussLegendre(g, 0.0, 2.0, 2);
    std::cout << res << std::endl;

    return 0;
}
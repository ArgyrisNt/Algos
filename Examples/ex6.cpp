// Example of interpolation algorithms

#include <iostream>
#include <fstream>
#include <string>
#include "..\NumericalAnalysis\Interpolation.h"

int main()
{
    NumericalAnalysis::Interpolation interpolate;
    std::vector<std::pair<double, double>> dataset{{0.0, 0.0},{1.0, 0.8},{2.0, 1.0},{3.0, 0.2},{4.0, -0.7},{5.0, -1.0}};
    std::vector<std::pair<double, double>> interpolated_data = interpolate.linear(dataset);
    interpolate.plot("linear.dat");

    interpolated_data = interpolate.lagrange(dataset);
    interpolate.plot("lagrange.dat");

    interpolated_data = interpolate.newton(dataset);
    interpolate.plot("newton.dat");

    interpolated_data = interpolate.cubicSpline(dataset);
    interpolate.plot("cspline.dat");

    std::vector<double> times{0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
    interpolated_data = interpolate.bezier(dataset, times);
    interpolate.plot("bezier.dat");

    return 0;
}
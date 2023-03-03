#include <iostream>
#include <vector>
#include "Matrix.h"

namespace NumericalAnalysis
{

    class ODEsolvers
    {
    public:
        std::vector<double> forwardEuler(double (*func)(double, double), double y0, double step);
        std::vector<double> backwardEuler(double (*func)(double, double), double y0, double step);
        std::vector<double> RK4(double (*func)(double, double), double y0, double step);
    };

} // namespace NumericalAnalysis

#include "ODEsolvers.cpp"
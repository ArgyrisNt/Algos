// Example of ode and pde solvers

#include <iostream>
#include <vector>
#include "..\NumericalAnalysis\ODEsolvers.h"
#include "..\NumericalAnalysis\PDEsolvers.h"
#include "..\NumericalAnalysis\Matrix.h"

double f(double t, double x)
{
    return x;
}

int main()
{
    NumericalAnalysis::ODEsolvers odesolver;
    std::vector<double> values = odesolver.forwardEuler(f, 1.0, 1.0);
    // std::vector<double> values = odesolver.backwardEuler(f, 1.0, 0.5);
    // std::vector<double> values = odesolver.RK4(f, 1.0, 0.1);

    NumericalAnalysis::PDEsolvers pdesolver;
    // NumericalAnalysis::Matrix<double> U = pdesolver.heat_explicit(0.0, 0.0, 1.0, 800, 19);
    NumericalAnalysis::Matrix<double> U = pdesolver.heat_implicit(0.0, 0.0, 1.0, 800, 19);
    // NumericalAnalysis::Matrix<double> U = pdesolver.heat_CrankNicolson(0.0, 0.0, 1.0, 800, 19);
    //NumericalAnalysis::Matrix<double> U = pdesolver.advection_upwind(0.0, 0.0, 1.0, 250, 100);

    return 0;
}
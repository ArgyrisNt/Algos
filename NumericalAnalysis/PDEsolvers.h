#include <iostream>
#include <vector>
#include "Matrix.h"

namespace NumericalAnalysis
{

    class PDEsolvers
    {
    public:
        Matrix<double> heat_explicit(double x0, double x1, double t0, double Nt, double Nx); // parabolic
        Matrix<double> heat_implicit(double x0, double x1, double t0, double Nt, double Nx);
        Matrix<double> heat_CrankNicolson(double x0, double x1, double t0, double Nt, double Nx);
        Matrix<double> advection_upwind(double x0, double x1, double t0, double Nt, double Nx); // hyperbolic
    };

} // namespace NumericalAnalysis

#include "PDEsolvers.cpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

namespace NumericalAnalysis
{

    std::vector<double> ODEsolvers::forwardEuler(double (*func)(double, double), double y0, double step)
    {
        std::vector<double> res;
        double t = 0.0;
        double f0 = func(t, y0);
        while (t < 10.0)
        {
            double y1 = y0 + step * f0;
            res.push_back(y1);
            y0 = y1;
            f0 = func(t, y0);
            t += step;
        }

        return res;
    }

    std::vector<double> ODEsolvers::backwardEuler(double (*func)(double, double), double y0, double step)
    {
        // Newton-Rapson method
        std::vector<double> res;
        double t = 0.0;
        double dg = 1.0 - step;
        while (t < 10.0)
        {
            double y1_old = 3.0;
            double f1 = func(t, y1_old);
            double g1 = y1_old - step * f1 - y0;
            double y1 = y1_old - g1 / dg;
            res.push_back(y1);
            y0 = y1;
            t += step;
        }

        return res;
    }

    std::vector<double> ODEsolvers::RK4(double (*func)(double, double), double y0, double step)
    {
        std::vector<double> res;
        double t = 0.0;
        while (t < 10.0)
        {
            double k1 = func(t, y0);
            double k2 = func(t + step / 2.0, y0 + (step * k1) / 2.0);
            double k3 = func(t + step / 2.0, y0 + (step * k2) / 2.0);
            double k4 = func(t + step, y0 + step * k3);
            double y1 = y0 + (step / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
            res.push_back(y1);
            y0 = y1;
            t += step;
        }

        return res;
    }

}